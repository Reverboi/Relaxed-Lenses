#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "lente.hpp"
#define e 2e-10 // numero piccolo: 'e' sta per epsilon
#define e1 2e-4 // numero piccolo: 'e' sta per epsilon
#define e2 4e-21 // numero piccolo: 'e' sta per epsilon
#define MAX_LOOP 10000

double Snell(double angolo, double index){ //ritorna +- NAN se siamo in total internal reflection
	return asin(sin(angolo)/index);
	}

double Curva :: operator()(double x) const{
	if(Q.Size()==0) return 0;
	double result=Q.[0];
	double c=x*PI/Campo;
	for(int i=1;i<Q.size();i++){
		result+=Q[i]*cos(i*x*)/i;
		c *= x*x;
		}
	return result;
	}

double Curva:: Angolo(double x){return atan(Derivata(x));}

double Curva::Derivata(double x){
	double result=0.0;
	double c=x;
	for(int i=1;i<Q.size();i++){
		result += 2*i*c*Q[i];
		c *= x*x;
		}
	return result;
	}

double Curva:: Intersect(Raggio in){
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

Raggio Lente :: Out (Raggio I){ //non log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(x, Inf(x), Snell(I.A-Inf.Angolo(x),N)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N)+Sup.Angolo(x));
	}

void Raggio :: Log(std::ofstream& fpt){
	if(fpt.is_open()) fpt<<X<<" "<<Y<<"\n";
	}
	
void Curva :: Log(std::ofstream& fpt){
	if(fpt.is_open()) 
	for(int i=0;i<Q.size();i++){
		fpt<<Q[i]<<"*x**"<<i*2<<"+";
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

double RandomUpdate(vector<Lente>& lente){
	const double x = rand()*CAMPO/RAND_MAX;
	double score=Score(lente,x);
	double safe;
	vector<Lente> temp = lente;
	for(int i=0;i<lente.size();i++){
		
		for(int j=0;j<lente[i].Inf.Q.size();j++){
			safe=lente[i].Inf.Q[j];
			lente[i].Inf.Q[j]+= e2;
			temp[i].Inf.Q[j] += (Score(lente,x)-score)*e1;
			lente[i].Inf.Q[j]=safe;
			}
		for(int j=0;j<lente[i].Sup.Q.size();j++){
			safe=lente[i].Sup.Q[j];
			lente[i].Sup.Q[j]+=e2;
			temp[i].Sup.Q[j] += (Score(lente,x)-score)*e1;
			lente[i].Sup.Q[j]=safe;
			}
		}
	lente=temp;
	return score;
	}

Raggio vector<Lente> :: Out (Raggio I){ 
	for(int i=0;i<size();i++){
		I = at(i).Out(A);
		}
	return I;
	}

Raggio vector<Lente> :: Out (std::ofstream& fpt,Raggio I){ //log version
	for(int i=0;i<size();i++){
		I = at(i).Out(fpt,A);
		}
	return I;
	}
	
double Score(vector<Lente>& sistema, const double x){
	Raggio in = Raggio(x,-ALTEZZA_SENSORE/10,0);
	double target = - x * DIMENSIONE_SENSORE / CAMPO;
	Raggio out = sistema.Out(in);
	double hit = (out.Y-ALTEZZA_SENSORE)*tan(out.A);
	if (hit!=hit) return -(CAMPO+DIMENSIONE_SENSORE)*(CAMPO+DIMENSIONE_SENSORE);
	double dis = hit - target;
	return -dis*dis;
	//return 1.0/(1.0+dis*dis);
	}

void Gnuplotta(vector<Lente> lente){
	Curva Sensore = Curva(ALTEZZA_SENSORE)
	for(int i=0; i < num_raggi; i++){		//creo i file per i singoli raggi
		ofstream fpt ("dati/"+to_string(i)+".dat");
		if ((fpt.is_open()) == false){
        	printf("Error! opening file");
        	exit(1);
    		}
   		Raggio ray = Raggio(fpt,-CAMPO+i*2*CAMPO/num_raggi,0,0.0);
		for(int i=0;i<lente.size();i++){
			Raggio a = lente[i].Out(fpt,ray);
			ray=a;
			}
		double x=Sensore.Intersect(ray);
		ray= Raggio(fpt,x,Sensore(x),0);
		fpt.close();
		}
	ofstream fpt ("data.gp");
	if ((fpt.is_open()) == false){
        printf("Error! opening file");
        exit(1);
    	}
    fpt<<("set size ratio -1\nset terminal pdf\nset output 'plot.pdf'\nset nokey\n");
    fpt<<("plot ["+to_string(-CAMPO)+":"+to_string(CAMPO)+"] [0:"+to_string(Sensore(0))+"] ").c_str();
    tele.Log(fpt);
    for(int i=0; i < num_raggi; i++){
    	fpt<<"'dati/"+to_string(i)+".dat' u 1:2 with lines";
    	if (i+1<num_raggi) fpt<<", ";
    	}
    fpt.close();
    system("gnuplot -p data.gp");
    }
