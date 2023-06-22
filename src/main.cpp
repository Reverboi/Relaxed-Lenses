#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>

#include "Sistema.hpp"
#include "Polinomio.hpp"
#include "Arco.hpp"


#define CAMPO 28
#define DIMENSIONE_SENSORE 3.3
#define ALTEZZA_SENSORE 150

#define INPUT_DIR std::string("../data/input/")

using namespace std;

// misure in mm

int main(){
    ifstream inf ( INPUT_DIR + string("inf.param") );
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
    ifstream sup ( INPUT_DIR + string("sup.param"));
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }

	Sistema current = Sistema( ALTEZZA_SENSORE, DIMENSIONE_SENSORE,  CAMPO );
	
	Polinomio dw1=Polinomio(inf);
	Polinomio up1=Polinomio(sup);
	current.InserisciLente(Lente( dw1, up1, 1.490 ,1.499));
	
	current.OttimizzaSensore();
	
	Arco dw0=Arco(10,24,-129);
	Arco up0=Arco(15,24,-124);
	//current.InserisciLente(Lente( dw0, up0, 1.717, 1.7395));
	for(int i=0;i<20;i++) current.OttimizzaLente(1);
	//current.OttimizzaSensore();
	
	inf.close();
        sup.close();
	cout.precision(7);
	
	
	current.Gnuplotta("plot.pdf");
	cout<<endl<<"... COMPLETED!.."<<endl;
	}
