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
#define DIMENSIONE_SENSORE 0
#define ALTEZZA_SENSORE 140
#define MAX_LOOP 1<<12
#define FINAL ldexp(1.0,-100)
#define DOWN 1.2
#define UP 8
using namespace std;

// misure in mm

int main(){
        vector<Lente> pino;
        Lente telec = Lente(Curva(-10,CAMPO,ORD),Curva(10,CAMPO,ORD),1.55,1.56);
        Lente crom = Lente(Curva(12,CAMPO,ORD),Curva(18,CAMPO,ORD),1.70,1.68);
        pino.push_back(telec);
        pino.push_back(crom);
	Sistema* current= new Sistema(pino, CAMPO, ALTEZZA_SENSORE, DIMENSIONE_SENSORE);
	Sistema* next= new Sistema(*current);
	cout.precision(7);
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
				fpt<<log10(eps)<<std::endl;
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
	Gnuplotta(*current);
	
	cout<<endl<<"... COMPLETED!.."<<endl;
	
	
	}
	

