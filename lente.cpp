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
	double result=0.0;
	double c=1.0;
	for(int i=0;i<Q.size();i++){
		result+=Q[i]*c;
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
	
Raggio Lente :: Out (std::ofstream& fpt, Raggio I){ //log version
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
double Sistema :: Update(double x){
	Raggio X = Raggio(x,0,0);
	double score=Score(X);
	double safe;
	Sistema nuovo = *this; 
	for(int i=0;i<lente.size();i++){
		
		for(int j=0;j<lente[i].Inf.Q.size();j++){
			safe=lente[i].Inf.Q[j];
			lente[i].Inf.Q[j]+= e2;
			nuovo.lente[i].Inf.Q[j] += (Score(X)-score)*e1;
			lente[i].Inf.Q[j]=safe;
			}
		for(int j=0;j<lente[i].Sup.Q.size();j++){
			safe=lente[i].Sup.Q[j];
			lente[i].Sup.Q[j]+=e2;
			nuovo.lente[i].Sup.Q[j] += (Score(X)-score)*e1;
			lente[i].Sup.Q[j]=safe;
			}
		}
	*this=nuovo;
	return score;
	}

Raggio Sistema :: Out(Raggio I){
	Raggio A = I;
	for(int i=0;i<lente.size();i++){
		Raggio B = lente[i].Out(A);
		A=B;
		}
	return A;
	}
Raggio Sistema :: Out (std::ofstream& fpt, Raggio I){ //log version
	Raggio A = I;
	for(int i=0;i<lente.size();i++){
		Raggio B = lente[i].Out(fpt,A);
		A=B;
		}
	return A;
	}
	
double Sistema :: Score(Raggio in){
	if (in.A!=in.A) return -(Campo+DimensioneSensore)*(Campo+DimensioneSensore);
	double target = - in.X * DimensioneSensore / Campo;
	double hit = Sensore.Intersect(Out(in));
	if (hit!=hit) return -(Campo+DimensioneSensore)*(Campo+DimensioneSensore);
	double dis = hit - target;
	return -dis*dis;
	//return 1.0/(1.0+dis*dis);
	}
