#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>

#include "Sistema.hpp"
#include "Polinomio.hpp"
#include "Arco.hpp"

#define EPS 0.5
#define ORD 2
#define CAMPO 28
#define DIMENSIONE_SENSORE 3.3
#define DESTINATION "../plot.pdf"
#define ALTEZZA_SENSORE 150
#define FINAL ldexp(1.0,-100)
#define DOWN 1.2
#define UP 8
using namespace std;

// misure in mm

int main(){
        ifstream inf ("..//param.inf");
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
        ifstream sup ("..//param.sup");
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
	Sistema current = Sistema( ALTEZZA_SENSORE, DIMENSIONE_SENSORE,  CAMPO );
	
	Polinomio dw1=Polinomio(inf);
	Polinomio up1=Polinomio(sup);
	current.InserisciLente(Lente( dw1, up1, 1.4756 ,1.47555));
	
	current.OttimizzaSensore();
	
	Arco dw0=Arco(10,24,-129);
	Arco up0=Arco(40,24,-99);
	current.InserisciLente(Lente( dw0, up0, 1.35, 1.6));
	for(int i=0;i<20;i++) current.OttimizzaLente(1);
	current.OttimizzaSensore();
	
	inf.close();
        sup.close();
	cout.precision(7);
	
	
	current.Gnuplotta(DESTINATION);
	cout<<endl<<"... COMPLETED!.."<<endl;
	}
