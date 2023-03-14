#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"

#define ORD 2
#define CAMPO 400
#define DIMENSIONE_SENSORE 10
#define ALTEZZA_SENSORE 4000
#define MAX_LOOP 1<<10
using namespace std;

// misure in mm

int main(){

	Sistema lemielenti = Sistema({1.5},CAMPO, ALTEZZA_SENSORE, DIMENSIONE_SENSORE, ORD);
	cout.precision(17);
	cout<<"...PROCESSING..."<<endl;
	for(int i=0;i<lemielenti.lente.size();i++){
	for(int j=1;j<ORD;j++){
		GlobalUpdate(lemielenti, i, j);
		}
		}
	cout<<endl<<"... COMPLETED!.."<<endl;
	
	Gnuplotta(lemielenti);
	}
	

