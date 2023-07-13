#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>

#include "Sistema.hpp"
#include "Polinomio.hpp"
#include "Arco.hpp"
#include "Punti.hpp"


#define CAMPO 28
#define AIR 1.003
#define DIMENSIONE_SENSORE  3.3
#define ALTEZZA_SENSORE 150

#define INPUT_DIR std::string("../data/input/")

using namespace std;

// misure in mm

int main(){
    ifstream inf ( INPUT_DIR + string("downPoints.dat") );
	if ((inf.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }
    ifstream sup ( INPUT_DIR + string("upPoints.dat"));
	if ((sup.is_open()) == false){
            printf("Error! opening file");
            exit(1);
        }

	Sistema current = Sistema( ALTEZZA_SENSORE, DIMENSIONE_SENSORE,  CAMPO );
	
	Punti* dw0 = new Punti(inf ,0, 35, AIR,   1.499, AIR,   1.49);
	Punti* up0 = new Punti(sup, 0, 35, 1.499, AIR,   1.49, AIR);

	//current.InserisciLente(Lente( dw0, up0, 1.490 ,1.499));   //veri valori PMMA
	current.InserisciElemento(dw0);
	current.InserisciElemento(up0);
	inf.close();
	sup.close();
	current.OttimizzaSensore();
	current.Gnuplotta("single.pdf");
	

	ifstream infi (INPUT_DIR + string("upPoints.dat"));
	if ((infi.is_open()) == false) {
		printf("Error! opening file");
		exit(1);
	}
	ifstream supi (INPUT_DIR + string("downPoints.dat"));
	if ((supi.is_open()) == false) {
		printf("Error! opening file");
		exit(1);
	}

	Punti* dw1 = new Punti(infi, 130.5,- 3.3, AIR, 1.499, AIR, 1.49);
	Punti* up1 = new Punti(supi, 130.5,- 3.3, 1.499, AIR, 1.49, AIR);

	//current.InserisciLente(Lente( dw0, up0, 1.490 ,1.499));   //veri valori PMMA
	current.InserisciElemento(dw1);
	current.InserisciElemento(up1);
	infi.close();
	supi.close();
	current.AltezzaSensore += 25;
	current.OttimizzaSensore();
	/*
	up0->IndiceBluSopra = 1.603;
	up0->IndiceRossoSopra = 1.5847;
	Arco* dw1 = new Arco(8, 24, 450, 1.5847, AIR, 1.603, AIR);
	
	current.InserisciElemento(dw1);
	//current.OttimizzaSensore();
	for (int i = 0; i < 30; i++) {
		current.OttimizzaElemento(2);
	}
	*/

	//current.OttimizzaSensore();
	
	current.Gnuplotta("double.pdf");
	cout<<endl<<"... COMPLETED!.."<<endl;
	}
