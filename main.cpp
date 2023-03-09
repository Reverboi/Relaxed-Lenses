#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"
#define num_raggi 20
#define ORD 2
#define CAMPO 400
#define DIMENSIONE_SENSORE 10
#define ALTEZZA_SENSORE 1000
#define MAX_LOOP 600000
using namespace std;

// misure in mm

int main(){

	vector<Lente> sistema={1.5};
	cout<<"...PROCESSING..."<<endl;
	for(int i=0;i<MAX_LOOP;++i){
		RandomUpdate(lemielenti);
		//std::cout<<tele.Update(tele.Campo*rand()/RAND_MAX)<<std::endl;
		}
	cout<<"... COMPLETED!.."<<endl;
	
	Gnuplotta(sistema);
	}
	

