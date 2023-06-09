#include "Sistema.hpp"
#define e ldexp(1.0,-50)

void OttimizzaLente(int i) {
    if ((i<0)||(i>=lente.size())) return; // indice lente invalido
    Lente& s = lente[i];
            
    
    }

Raggio Sistema :: Out_d (Raggio I) const { 
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_d(I);
		}
	return I;
	}

Raggio Sistema :: Out_d (std::ofstream& fpt,Raggio I) const { // log version
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_d(fpt,I);
		}
	return I;
	}
	
Raggio Sistema :: Out_f (Raggio I) const { 
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_f(I);
		}
	return I;
	}

Raggio Sistema :: Out_f (std::ofstream& fpt,Raggio I) const { // log version
	for(int i=0;i<lente.size();i++){
		I = lente.at(i).Out_f(fpt,I);
		}
	return I;
	}
	
double Sistema :: Score_d(const double x) const {
	Raggio in = Raggio(x,-AltezzaSensore/2,0);
	double target = - x * DimensioneSensore / Campo;
	Raggio out = Out_d(in);
	double hit = out.X+(out.Y-AltezzaSensore)*tan(out.A);
	if (hit!=hit) return (Campo+DimensioneSensore);
	return hit - target;
	}

double Sistema :: Score_f(const double x)  const {
	Raggio in = Raggio(x,-AltezzaSensore/2,0);
	double target = - x * DimensioneSensore / Campo;
	Raggio out = Out_f(in);
	double hit = out.X+(out.Y-AltezzaSensore)*tan(out.A);
	if (hit!=hit) return (Campo + DimensioneSensore);   // don't ask
	return hit - target;
	}

double Sistema :: GScore() const {
	double res=0.0;
	int G=RISOLUZIONE_G_SCORE;
	for(int i=0;i<G;i++){
		double  x = -Campo + i * Campo/G;
		double s1 = Score_d(x);
		double s2 = Score_f(x);
		res -= s1*s1 + s2*s2;
		}
	return res/(2*G*DimensioneSensore*DimensioneSensore); // normalizzato sulla dimensione del sensore
	}
	
void Sistema :: Gnuplotta(std::string destination) const {
        std::ofstream sens ("../dati/sensore.dat");
        sens<<-DimensioneSensore<<" "<<AltezzaSensore<<"\n";
        sens<< DimensioneSensore<<" "<<AltezzaSensore;
        sens.close();
        
	for(int i=1; i < num_raggi; i++){		//creo i file per i singoli raggi
		std::ofstream fpt ("../dati/"+std::to_string(i)+"_d.dat");   //d
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
		
		fpt = std::ofstream("../dati/"+std::to_string(i)+"_f.dat");     //f
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
	std::ofstream fpt ("../data.gp");
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
    	fpt<<"'../dati/"<<i<<"_d.dat' u 1:2 with lines lt rgb "<<'"'<<"orange"<<'"';
    	fpt<<", ";
    	}
    fpt<<" '../dati/sensore.dat' u 1:2 with lines lt rgb "<<'"'<<"green"<<'"';
    
    fpt<<"\nplot [-"<<Campo<<":"<<Campo<<"] [-20:"<<AltezzaSensore+20<<"] ";
    
    Log(fpt);
    
    for(int i=1; i < num_raggi; i++){
    	fpt<<"'../dati/"<<i<<"_f.dat' u 1:2 with lines lt rgb "<<'"'<<"blue"<<'"';
    	/*if (i+1<num_raggi)*/ fpt<<", ";
    	}
    fpt<<" '../dati/sensore.dat' u 1:2 with lines lt rgb "<<'"'<<"green"<<'"';
    
    fpt<<"\nset xtics\n";
    fpt<<"set xlabel "<<'"'<<"Campo inquadrato (mm)"<<'"'<<"\n";
    fpt<<"set ylabel "<<'"'<<"errore offset raggio (mm)"<<'"'<<"\n";
    fpt<<("\nset size noratio\n");
    fpt<<"plot [:][:] '../scores.dat' u 1:2 with lines lt rgb"<<'"'<<"orange"<<'"'
    <<", '../scores.dat' u 1:3 with lines lt rgb"<<'"'<<"blue"<<'"'<<"\n";
    fpt<<"set ylabel "<<'"'<<"errore offset raggio (log10-scale) "<<'"'<<"\n";
    fpt<<"plot [:][:] '../scores.dat' u 1:4 with lines\n";
    //fpt<<"plot [:][:] 'eps.dat' with lines\n";
    fpt.close();
    std::ofstream fpy ("../scores.dat");
    
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
    system("gnuplot -p ../data.gp"); // non funzionerà su windows forse
    }

void Sistema :: OttimizzaSensore() {    
    double fx,bx;
    double cx = GScore();
    double crawl = AltezzaSensore/2.0; 
    int i=0;
    for(;(i<24)&&(crawl*crawl >= e);i++){
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

Sistema :: Sistema(const Sistema& source) : AltezzaSensore(source.AltezzaSensore), DimensioneSensore(source.DimensioneSensore), Campo(source.Campo){
    for(Lente s : source.lente) lente.push_back(Lente(s)); 
    }

void Sistema :: InserisciLente(const Lente& q){
    lente.push_back(q);
    //std::sort(lente.begin(),lente.end(),[](const Lente& a, const Lente& b){return a.Inf.Quota < b.Inf.Quota;});
	}

void Sistema :: Log(std::ofstream &fpt) const {
	for(int i=0;i<lente.size();i++){
		lente[i].Log(fpt);
		}
	}
