#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "lente.hpp"
#define e  ldexp(1.0,-32)  // numero piccolo: 'e' sta per epsilon
#define e1 ldexp(1.0,-1)   // numero piccolo: 'e' sta per epsilon
#define e2 ldexp(1.0,-12)  // numero piccolo: 'e' sta per epsilon
#define MAX_LOOP 10000
#define PI 3.14159265359
#define num_raggi 20
double Snell(double angolo, double index){ //ritorna +- NAN se siamo in total internal reflection
	return asin(sin(angolo)/index);
	}

double Curva :: operator()(double x) const{
	if(Q.size()==0) return 0;
	double result=Q[0];
	
	double c=x*PI/(Ampiezza*2);
	for(int i=1;i<Q.size();i++){
		result+=Q[i]*cos(i*c)/i;
		}
	return result;
	}

double Curva:: Angolo(double x){return atan(Derivata(x));}

double Curva::Derivata(double x){
	if(Q.size()==0) return 0;
	double result=0.0;
	double c=x*PI/(Ampiezza*2);
	for(int i=1;i<Q.size();i++){
		result -= Q[i]*sin(i*c)*PI/(Ampiezza*2);
		}
	return result;
	}

double Curva:: Intersect(const Raggio& in){
	if(in.A!=in.A) return NAN; //total internal reflection
	if(in.Y>((*this)(in.X))){  // ...se il raggio è già oltre...
		return NAN; 
		}
	double r=1000.0;
	double const xx=-sin(in.A);
	double const yy=cos(in.A);
	double x0,y0,x1=in.X,y1=in.Y;
	for(int i=0;i<MAX_LOOP;i++){
		if ((y1-(*this)(x1))*(y1-(*this)(x1))<=e*e) break;
		if (y1>(*this)(x1)) r=r/2;
		else {
			x0=x1;
			y0=y1;
			}
		x1=x0+r*xx;
		y1=y0+r*yy;
		}
	return x1;
	}
	
Raggio Lente :: Out (std::ofstream& fpt, const Raggio& I){ //log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(fpt, x, Inf(x), Snell(I.A-Inf.Angolo(x),N)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(fpt,x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N)+Sup.Angolo(x));
	}

Raggio Lente :: Out (const Raggio& I){ //non log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(x, Inf(x), Snell(I.A-Inf.Angolo(x),N)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N)+Sup.Angolo(x));
	}

void Raggio :: Log(std::ofstream& fpt){
	if(fpt.is_open()) fpt<<X<<" "<<Y<<"\n";
	}
	
void Curva :: Log(std::ofstream& fpt){
	if(fpt.is_open()){
		fpt<<Q[0]<<" + ";
		for(int i=1;i<Q.size();i++){
			fpt<<Q[i]<<"*cos(x*"<<PI*i/(2*Ampiezza)<<")/"<<i<<" + ";
			}
		}
	fpt<<"0, ";
	}

void Lente :: Log(std::ofstream &fpt){
	Inf.Log(fpt);
	//fpt<<" title ' ', ";
	Sup.Log(fpt);
	//fpt<<" title ' ', ";
	}

void Sistema :: Log(std::ofstream &fpt){
	for(int i=0;i<lente.size();i++){
		lente[i].Log(fpt);
		}
	}

double RandomUpdate(Sistema& D){
	const double x = rand()*D.Campo/RAND_MAX;
	double sfx,sbx;
	Sistema fx = D;
	Sistema bx = D;
	for(int i=0;i<D.lente.size();i++){
		
		for(int j=1;j<D.lente[i].Inf.Q.size();j++){
			sfx=fx.lente[i].Inf.Q[j];
			sbx=bx.lente[i].Inf.Q[j];
			fx.lente[i].Inf.Q[j] += e2;
			bx.lente[i].Inf.Q[j] -= e2;
			D.lente[i].Inf.Q[j]  += (Score(fx,x)-Score(bx,x))*e1;
			fx.lente[i].Inf.Q[j] = sfx;
			bx.lente[i].Inf.Q[j] = sbx;
			}

		for(int j=1;j<D.lente[i].Sup.Q.size();j++){
			sfx=fx.lente[i].Sup.Q[j];
			sbx=bx.lente[i].Sup.Q[j];
			fx.lente[i].Sup.Q[j] += e2;
			bx.lente[i].Sup.Q[j] -= e2;
			D.lente[i].Sup.Q[j]  += (Score(fx,x)-Score(bx,x))*e1;
			fx.lente[i].Sup.Q[j]=sfx;
			bx.lente[i].Sup.Q[j]=sbx;
			}
		}
	return Score(D,x);
	}

Raggio Sistema :: Out (Raggio I){ 
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out(I);
		}
	return I;
	}

Raggio Sistema :: Out (std::ofstream& fpt,Raggio I){ //log version
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out(fpt,I);
		}
	return I;
	}
	
double Score(Sistema& D, const double x){
	Raggio in = Raggio(x,-D.AltezzaSensore/10,0);
	double target = - x * D.DimensioneSensore / D.Campo;
	Raggio out = D.Out(in);
	double hit = out.X+(out.Y-D.AltezzaSensore)*tan(out.A);
	if (hit!=hit) return -(D.Campo+D.DimensioneSensore)*(D.Campo+D.DimensioneSensore);
	double dis = hit - target;
	return -dis*dis;
	//return 1.0/(1.0+dis*dis);
	}
	
void Gnuplotta(Sistema& D){
	for(int i=1; i < num_raggi; i++){		//creo i file per i singoli raggi
		std::ofstream fpt ("dati/"+std::to_string(i)+".dat");
		if ((fpt.is_open()) == false){
        	printf("Error! opening file");
        	exit(1);
    		}
   		Raggio ray = Raggio(fpt,-D.Campo+i*2*D.Campo/num_raggi,0,0.0);
		for(int i=0;i<D.lente.size();i++){
			Raggio a = D.lente[i].Out(fpt,ray);
			ray=a;
			}
		double x=D.Sensore.Intersect(ray);
		ray= Raggio(fpt,x,D.Sensore(x),0);
		fpt.close();
		}
	std::ofstream fpt ("data.gp");
	if ((fpt.is_open()) == false){
        printf("Error! opening file");
        exit(1);
    	}/*
    fpt<<("set terminal pdf\nset output 'errors.pdf'\nset nokey\nplot 'scores.dat' u 1:2 with lines\nset output 'errlogs.pdf'\nplot 'scores.dat' u 1:3 with lines\n");
    fpt<<("set size ratio -1\nset output 'plot.pdf'\n");
    fpt<<("plot ["+std::to_string(-D.Campo)+":"+std::to_string(D.Campo)+"] [0:"+std::to_string(D.AltezzaSensore)+"] ").c_str();
    */
    fpt<<("set terminal pdf\nset output 'plot.pdf'\nset nokey\n");
    fpt<<("set size ratio -1\n");
    fpt<<("plot ["+std::to_string(-D.Campo)+":"+std::to_string(D.Campo)+"] [0:"+std::to_string(D.AltezzaSensore)+"] ").c_str();
    D.Log(fpt);
    for(int i=1; i < num_raggi; i++){
    	fpt<<"'dati/"+std::to_string(i)+".dat' u 1:2 with lines";
    	if (i+1<num_raggi) fpt<<", ";
    	}
    fpt<<("\nset size noratio\n");
    fpt<<("plot 'scores.dat' u 1:2 with lines\nplot 'scores.dat' u 1:3 with lines\n");
    fpt.close();
    std::ofstream fpy ("scores.dat");
	if ((fpy.is_open()) == false){
        printf("Error! opening file");
        exit(1);
    	}
    int pti=100;
    for(int i=1; i < pti; i++){
    double x=-D.Campo+i*2*D.Campo/pti;
    double t=-Score(D,x);
    fpy<< x <<' '<< t <<' '<< log(t) <<std::endl;
    }
    fpy.close();
    system("gnuplot -p data.gp");
    }
