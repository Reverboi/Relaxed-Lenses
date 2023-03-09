#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"

#define ORD 10
#define CAMPO 400
#define DIMENSIONE_SENSORE 10
#define ALTEZZA_SENSORE 4000
#define MAX_LOOP 1<<9
using namespace std;

// misure in mm

int main(){

	Sistema lemielenti = Sistema({1.5,1.77,1.5},CAMPO, ALTEZZA_SENSORE, DIMENSIONE_SENSORE, ORD);
	cout<<"...PROCESSING..."<<endl;
	for(int i=0;i<MAX_LOOP;++i){
		/*if(i%(MAX_LOOP/10)==0)cout<<RandomUpdate(lemielenti)<<endl;
		else*/ RandomUpdate(lemielenti);
		}
	cout<<endl<<"... COMPLETED!.."<<endl;
	
	Gnuplotta(lemielenti);
	}
	

