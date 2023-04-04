#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"

#define EPS ldexp(1.0,-40)
#define ORD 3
#define CAMPO 400
#define DIMENSIONE_SENSORE 10
#define ALTEZZA_SENSORE 4000
#define MAX_LOOP 1<<13
#define FINAL ldexp(1.0,-100)
#define DOWN 1.2
#define UP 8
#define BLOW ldexp(1.0,-62)
#define BLOW_TIMES 1
using namespace std;

// misure in mm

int main(){

	Sistema* current= new Sistema({1.5,1.2},CAMPO, ALTEZZA_SENSORE, DIMENSIONE_SENSORE, ORD);
	Sistema* next= new Sistema(*current);
	cout.precision(7);
	cout<<"...PROCESSING..."<<endl;
	double top=-10000.0;
	double eps=EPS;
	for(int h=0;h<BLOW_TIMES;h++){
	for(int j=0;j<=MAX_LOOP;j++){
		GlobalUpdate(*current,*next,eps);
		if((GScore(*next)!=GScore(*next))||(GScore(*next)<=GScore(*current))){
			eps=eps/UP;
			if (eps<=FINAL){
				cout<<"final"<<endl;
				break;
				}
			}
		else{
			*current=Sistema(*next);
			eps=eps*DOWN;
			}
		}
	cout<<GScore(*current)<<endl;
	if (GScore(*current)>top){	
		Gnuplotta(*current);
		top=GScore(*current);
		}
	GlobalUpdate(*current,*next,BLOW);
	*current=Sistema(*next);
	}	
	cout<<endl<<"... COMPLETED!.."<<endl;
	}
	

