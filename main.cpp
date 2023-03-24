#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"

#define EPS 0.1
#define ORD 4
#define CAMPO 400
#define DIMENSIONE_SENSORE 10
#define ALTEZZA_SENSORE 4000
#define MAX_LOOP 1<<12
#define FINAL ldexp(1.0,-100)
#define DOWN 1.2
#define UP 8
using namespace std;

// misure in mm

int main(){

	Sistema* current= new Sistema({1.5},CAMPO, ALTEZZA_SENSORE, DIMENSIONE_SENSORE, ORD);
	Sistema* next= new Sistema(*current);
	cout.precision(7);
	cout<<"...PROCESSING..."<<endl;
	double eps=EPS;
	for (int h=0; h<current->lente.size(); h++){
		for(int i=current->lente[h].Inf.Q.size()-1; i>0; i--){
		//for(int i=1; i<current->lente[h].Inf.Q.size(); i++){
			cout<<"doing:"<<i<<endl;
			for(int j=0;j<=MAX_LOOP;j++){
				GlobalUpdate(*current,*next,h,i,eps);
			
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
	/*
	for(int i=0;i<lemielenti.lente.size();i++){
		for(int j=1;j<ORD;j++){
			GlobalUpdate(lemielenti, i, j);
		}
	}
	*/
	cout<<endl<<"... COMPLETED!.."<<endl;
	
	
	}
	

