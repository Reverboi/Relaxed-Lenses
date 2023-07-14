#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>

#include "Sistema.hpp"

#define AIR 1.003

#define PLEXIGLASS_R	1.499
#define PLEXIGLASS_B	1.490

#define POLYCARBON_R	1.585
#define POLYCARBON_B	1.603

#define CAMPO 28
#define DIMENSIONE_SENSORE  3.3
#define ALTEZZA_SENSORE 150

#define INPUT_DIR std::string("../data/input/")

using namespace std;
using namespace RelaxedLenses;
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
	
	Punti* dw0 = current.NuovoPunti(inf ,0, 35, AIR, AIR,				  PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up0 = current.NuovoPunti(sup, 0, 35, PLEXIGLASS_R, PLEXIGLASS_B,					AIR, AIR);

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

	Punti* dw1 = new Punti(infi, 130.5,- 3.3, AIR, AIR, PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up1 = new Punti(supi, 130.5,- 3.3, PLEXIGLASS_R, PLEXIGLASS_B, AIR, AIR);

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
