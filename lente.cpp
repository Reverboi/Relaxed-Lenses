#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "lente.hpp"
#define e  ldexp(1.0,-100) // numero piccolo: 'e' sta per epsilon
#define e1 ldexp(1.0,-1)   // numero piccolo: 'e' sta per epsilon
#define e2 ldexp(1.0,-20)  // numero piccolo: 'e' sta per epsilon
#define e3 ldexp(1.0,-50)
#define MAX_LOOP 10000
#define PI 3.14159265359
#define num_raggi 40


double Snell(double angolo, double index){ //ritorna +- NAN se siamo in total internal reflection
	return asin(sin(angolo)/index);
	}

double Curva :: operator()(double x) const{
	double result=Quota;
        for(int i=0;i<Q.size();i++){
	  result += Q[i]*(pow(Ampiezza,2*(i+1))-pow(x,2*(i+1)))/pow(Ampiezza,2*(i+1));
	  }
	if(R!=0.0) result += sign(R)*(sqrt((1.0/(R*R)) - x*x) - sqrt((1.0/(R*R)) - Ampiezza*Ampiezza));
	return result;
	}

double Curva:: Angolo(double x){return atan(Derivata(x));}

double Curva::Derivata(double x){
	double result=0.0;
	for(int i=0;i<Q.size();i++){
	  result -= Q[i]*(i+1)*2*pow(x,i*2+1)/pow(Ampiezza,2*(i+1));
	  }
	if(R!=0.0) result += -x*sign(R)/(sqrt((1.0/(R*R)) - x*x));
	return result;
	}

std::string Numline(std::string str){     // Removes all leading non-number char until it finds a number
  std::string numStr="";
  for (int i=0; i<str.length(); i++) {
     if (isdigit(str[i]) || str[i] == '.' || str[i] == '-' || str[i] == 'e') {
         numStr = str.substr(i,str.length()-i);
         //std::cout<<numStr<<std::endl;
         break;
         }
     }
   return numStr;
   }
        

Curva :: Curva(std::ifstream& inp){
        std::string line;
        std::string numline;
        R=0.0;
        while (std::getline(inp, line)) {  // read each line from the file
            numline=Numline(line);
            char* pin;
            double num = std::strtod(numline.c_str(), &pin);
            std::string rest;
            switch(line[0]){
              case 'A':
                Ampiezza=num;
                break;
              case 'H':
                Quota=num;
                break;
              case 'R':
                R=1.0/num;
                break;
              case 'Q':
                Quota+=num;
                flag:
                rest = numline.substr(pin-&(numline[0]),10000);
                numline=Numline(rest);
                if (numline!=""){
                    num = std::strtod(numline.c_str(), &pin);
                    Q.push_back(num);
                    goto flag;
                    }
                break;
              default: break;
              }
            }
        }
        
double Curva:: Intersect(const Raggio& in){
	if(in.A!=in.A) return NAN; //total internal reflection
	if(in.Y>((*this)(in.X))){  // ...se il raggio è già oltre...
		return NAN; 
		}
	double r=1;
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
	
Raggio Lente :: Out_d (std::ofstream& fpt, const Raggio& I ){ //log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(fpt, x, Inf(x), Snell(I.A-Inf.Angolo(x),N_d)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(fpt,x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_d)+Sup.Angolo(x));
	}

Raggio Lente :: Out_d (const Raggio& I){ //non log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(x, Inf(x), Snell(I.A-Inf.Angolo(x),N_d)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_d)+Sup.Angolo(x));
	}

Raggio Lente :: Out_f (std::ofstream& fpt, const Raggio& I){ //log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(fpt, x, Inf(x), Snell(I.A-Inf.Angolo(x),N_f)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(fpt,x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_f)+Sup.Angolo(x));
	}

Raggio Lente :: Out_f (const Raggio& I){ //non log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(x, Inf(x), Snell(I.A-Inf.Angolo(x),N_f)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(x,Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_f)+Sup.Angolo(x));
	}
	
void Raggio :: Log(std::ofstream& fpt){
	if(fpt.is_open()) fpt<<X<<" "<<Y<<"\n";
	}
	
void Curva :: Log(std::ofstream& fpt){
	if(fpt.is_open()){
		fpt<<Quota<<" + ";
		for(int i=0;i<Q.size();i++){
			fpt<<Q[i]<<" * ("<<Ampiezza<<"**"<<(i+1)*2<<" - x**"<<(i+1)*2<<") / "<<Ampiezza<<"**"<<(i+1)*2<<" + ";
			}
		}
	if(R!=0.0) fpt<<"sgn("<<R<<")*(sqrt(1/("<<R*R<<") - x*x) - sqrt(1/("<<R*R<<") - "<<Ampiezza*Ampiezza<<")), ";
	else       fpt<<"0, ";
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

void GlobalUpdate(Sistema& D, Sistema& R, double eps){
	double sfx,sbx;
	Sistema fx = D;
	Sistema bx = D;
	R = Sistema(D);
	for(int i=0;i<D.lente.size();i++){
		for(int j=0;j<D.lente[i].Inf.Q.size();j++){
			sfx=fx.lente[i].Inf.Q[j];
			sbx=bx.lente[i].Inf.Q[j];
			fx.lente[i].Inf.Q[j] += e2;
			bx.lente[i].Inf.Q[j] -= e2;
			R.lente[i].Inf.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2*(j+1));
			fx.lente[i].Inf.Q[j] = sfx;
			bx.lente[i].Inf.Q[j] = sbx;
			}
		for(int j=0;j<D.lente[i].Sup.Q.size();j++){
			sfx=fx.lente[i].Sup.Q[j];
			sbx=bx.lente[i].Sup.Q[j];
			fx.lente[i].Sup.Q[j] += e2;
			bx.lente[i].Sup.Q[j] -= e2;
			R.lente[i].Sup.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2*(j+1));
			fx.lente[i].Sup.Q[j]=sfx;
			bx.lente[i].Sup.Q[j]=sbx;
			}
		}
	}

void GlobalUpdate(Sistema& D, Sistema& R, int i, int j, double eps){
	if (j<0) return;
	if (i<0) return;
	double sfx,sbx;
	Sistema fx = D;
	Sistema bx = D;
	R = Sistema(D);
		if(j<D.lente[i].Inf.Q.size()){
			sfx=fx.lente[i].Inf.Q[j];
			sbx=bx.lente[i].Inf.Q[j];
			fx.lente[i].Inf.Q[j] += e2;
			bx.lente[i].Inf.Q[j] -= e2;
			R.lente[i].Inf.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2);
			fx.lente[i].Inf.Q[j] = sfx;
			bx.lente[i].Inf.Q[j] = sbx;
			}

		if(j<D.lente[i].Sup.Q.size()){
			sfx=fx.lente[i].Sup.Q[j];
			sbx=bx.lente[i].Sup.Q[j];
			fx.lente[i].Sup.Q[j] += e2;
			bx.lente[i].Sup.Q[j] -= e2;
			R.lente[i].Sup.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2);
			fx.lente[i].Sup.Q[j]=sfx;
			bx.lente[i].Sup.Q[j]=sbx;
			}
	}


Raggio Sistema :: Out_d (Raggio I){ 
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_d(I);
		}
	return I;
	}

Raggio Sistema :: Out_d (std::ofstream& fpt,Raggio I){ // log version
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_d(fpt,I);
		}
	return I;
	}
	
Raggio Sistema :: Out_f (Raggio I){ 
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_f(I);
		}
	return I;
	}

Raggio Sistema :: Out_f (std::ofstream& fpt,Raggio I){ // log version
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_f(fpt,I);
		}
	return I;
	}
	
double Sistema :: Score_d(const double x){
	Raggio in = Raggio(x,-AltezzaSensore/2,0);
	double target = - x * DimensioneSensore / Campo;
	Raggio out = Out_d(in);
	double hit = out.X+(out.Y-AltezzaSensore)*tan(out.A);
	if (hit!=hit) return (Campo+DimensioneSensore);
	return hit - target;
	}

double Sistema :: Score_f(const double x){
	Raggio in = Raggio(x,-AltezzaSensore/2,0);
	double target = - x * DimensioneSensore / Campo;
	Raggio out = Out_f(in);
	double hit = out.X+(out.Y-AltezzaSensore)*tan(out.A);
	if (hit!=hit) return (Campo + DimensioneSensore);   // don't ask
	return hit - target;
	}

double Sistema :: GScore(){
	double res=0.0;
	int G=128;
	for(int i=0;i<G;i++){
		double  x = -Campo + i * Campo/G;
		double s1 = Score_d(x);
		double s2 = Score_f(x);
		res -= s1*s1 + s2*s2;
		}
	return res/(2*G*DimensioneSensore*DimensioneSensore); // normalizzato sulla dimensione del sensore
	}
	
void Sistema :: Gnuplotta(std::string destination){

        std::ofstream sens ("dati/sensore.dat");
        sens<<-DimensioneSensore<<" "<<AltezzaSensore<<"\n";
        sens<< DimensioneSensore<<" "<<AltezzaSensore;
        sens.close();
        
	for(int i=1; i < num_raggi; i++){		//creo i file per i singoli raggi
		std::ofstream fpt ("dati/"+std::to_string(i)+"_d.dat");   //d
		if ((fpt.is_open()) == false){
        	printf("Error! opening file");
        	exit(1);
    		}
   		Raggio ray = Raggio(fpt,-Campo+i*2*Campo/num_raggi,-AltezzaSensore/2,0.0);
		for(int i=0;i<lente.size();i++){
			Raggio a = lente[i].Out_d(fpt,ray);
			ray=a;
			}
		fpt << ray.X+(ray.Y-AltezzaSensore)*tan(ray.A) <<" "<<AltezzaSensore;
		fpt.close();
		
		fpt = std::ofstream("dati/"+std::to_string(i)+"_f.dat");     //f
		if ((fpt.is_open()) == false){
        	printf("Error! opening file");
        	exit(1);
    		}
   		ray = Raggio(fpt,-Campo+i*2*Campo/num_raggi,-AltezzaSensore/2,0.0);
		for(int i=0;i<lente.size();i++){
			Raggio a = lente[i].Out_f(fpt,ray);
			ray=a;
			}
		fpt << ray.X+(ray.Y-AltezzaSensore)*tan(ray.A) <<" "<< AltezzaSensore;
		fpt.close();
		}
	std::ofstream fpt ("data.gp");
	if ((fpt.is_open()) == false){
        printf("Error! opening file");
        exit(1);
    	}
    	
    fpt<<("set terminal pdf\nset output '"+destination+"'\nset nokey\n");
    fpt<<("set size ratio -1\n");
    fpt<<"set xlabel "<<'"'<<"Punteggio globale: "<<GScore()<<'"'<<"\n";
    fpt<<"unset xtics\n";
    
    fpt<<"plot [-"<<Campo<<":"<<Campo<<"] [-20:"<<AltezzaSensore+20<<"] ";
    Log(fpt);
    for(int i=1; i < num_raggi; i++){
    	fpt<<"'dati/"<<i<<"_d.dat' u 1:2 with lines lt rgb "<<'"'<<"orange"<<'"';
    	/*if (i+1<num_raggi)*/ fpt<<", ";
    	}
    fpt<<" 'dati/sensore.dat' u 1:2 with lines lt rgb "<<'"'<<"green"<<'"';
     fpt<<"\nplot [-"<<Campo<<":"<<Campo<<"] [-20:"<<AltezzaSensore+20<<"] ";
    
    Log(fpt);
    for(int i=1; i < num_raggi; i++){
    	fpt<<"'dati/"<<i<<"_f.dat' u 1:2 with lines lt rgb "<<'"'<<"blue"<<'"';
    	/*if (i+1<num_raggi)*/ fpt<<", ";
    	}
    fpt<<" 'dati/sensore.dat' u 1:2 with lines lt rgb "<<'"'<<"green"<<'"';
    
    fpt<<"\nset xtics\n";
    fpt<<"set xlabel "<<'"'<<"Campo inquadrato (mm)"<<'"'<<"\n";
    fpt<<"set ylabel "<<'"'<<"errore offset raggio (mm)"<<'"'<<"\n";
    fpt<<("\nset size noratio\n");
    fpt<<"plot [:][:] 'scores.dat' u 1:2 with lines lt rgb"<<'"'<<"orange"<<'"'
    <<", 'scores.dat' u 1:3 with lines lt rgb"<<'"'<<"blue"<<'"'<<"\n";
    fpt<<"set ylabel "<<'"'<<"errore offset raggio (log10-scale) "<<'"'<<"\n";
    fpt<<"plot [:][:] 'scores.dat' u 1:4 with lines\n";
    //fpt<<"plot [:][:] 'eps.dat' with lines\n";
    fpt.close();
    std::ofstream fpy ("scores.dat");
    
    if ((fpy.is_open()) == false){
    printf("Error! opening file");
    exit(1);
    }
    int pti=100;
    for(int i=1; i < pti; i++){
    	double x=-Campo+i*2*Campo/pti;
    	double t=Score_d(x);
    	double s=Score_f(x);
    	fpy<< x <<' '<< t <<' '<< s <<' '<< log10(sqrt(t*t+s*s)) <<std::endl;
    	}
    fpy.close();
    system("gnuplot -p data.gp");
    }
    
int sign(double f){
	if(f>0) return 1;
	if(f==0.0) return 0;
	else return -1;
	}
	
void Sistema :: OttimizzaSensore() {    
    double fx,bx;
    double cx = GScore();
    double crawl = AltezzaSensore/2.0; 
    int i=0;
    for(;(i<64)&&(crawl*crawl >= e3);i++){
      AltezzaSensore += crawl;
      fx = GScore();
      if ( fx > cx ) {
          cx=fx; 
          continue;
          }
      AltezzaSensore -= 2 * crawl;
      bx = GScore();
      if ( bx > cx ) {
          cx=bx;
          crawl = -crawl;
          continue;
          }
      else{
          AltezzaSensore += crawl;
          crawl = crawl / 2.0;
          }
      }
    }
