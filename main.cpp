#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"

#define EPS 0.5
#define ORD 2
#define CAMPO 35
#define DIMENSIONE_SENSORE 3.3
#define DESTINATION "plot.pdf"
#define ALTEZZA_SENSORE 150
#define MAX_LOOP 1<<12
#define FINAL ldexp(1.0,-100)
#define DOWN 1.2
#define UP 8
using namespace std;

// misure in mm

int main(){
        ifstream inf ("param.inf");
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
        ifstream sup ("param.sup");
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
	Sistema* current= new Sistema( ALTEZZA_SENSORE, DIMENSIONE_SENSORE,  CAMPO );
	Lente gian = Lente( new Polinomio(inf), new Polinomio(sup),1.4756,1.47555);   // Penso che come desing pattern sarebbe da fare che la lente ha cotruttore pubblico mentre la curva/polinomio
	//Lente gian = Lente(Curva(inf),Curva(sup),1.47,1.47);
	inf.close();
        sup.close();
	current->lente.push_back(gian);
	Sistema* next= new Sistema(*current);
	cout.precision(7);
	current->OttimizzaSensore();
	/*
	cout<<"...PROCESSING..."<<endl;
	double eps=EPS;
	std::ofstream fpt ("eps.dat");
	if ((fpt.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
        
	for (int h=0; h<current->lente.size(); h++){                   // per ogni lente nel sistema
		for(int i=current->lente[h].Inf.Q.size()-1; i>0; i--){    // per ogni cofficiente
			cout<<"doing:"<<i<<endl;
			for(int j=0;j<=MAX_LOOP;j++){
				GlobalUpdate(*current,*next,h,i,eps);
				fpt<<eps<<" "<<log10(eps)<<std::endl;
				if((GScore(*next)!=GScore(*next))||(GScore(*next)<=GScore(*current))){
					eps=eps/UP;
					if (eps<=FINAL){
						cout<<"Final"<<endl;
						eps=EPS/4;
						break;
						}
					}
				else{
					*current=Sistema(*next);
					eps=eps*DOWN;
					}
				}
			}
		}
	fpt.close();
	*/
	current->Gnuplotta(DESTINATION);
	cout<<endl<<"... COMPLETED!.."<<endl;
	
	
	}
	

