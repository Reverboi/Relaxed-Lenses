#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>

#include "Sistema.hpp"

#define AIR 1.0003

#define PLEXIGLASS_R	1.499
#define PLEXIGLASS_B	1.490
#define PLEXIGLASS		1.494

#define POLYCARBON_R	1.585
#define POLYCARBON_B	1.603

#define CAMPO 28
#define DIMENSIONE_SENSORE  3.4
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

	
	current.OttimizzaPosizioneSensore();
	current.Gnuplotta("single.pdf");
	inf.close();
	sup.close();
	inf.open(INPUT_DIR + string("downPoints.dat"));
	sup.open(INPUT_DIR + string("upPoints.dat"));

	Punti* dw1 = current.NuovoPunti(inf, 120.5, 4.0, AIR, AIR, PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up1 = current.NuovoPunti(sup, 120.5, 4.0, PLEXIGLASS_R, PLEXIGLASS_B, AIR, AIR);

	current.OttimizzaPosizioneLente(dw1, up1);
	//current.OttimizzaAmpiezzaLente(dw1, up1);
	//current.OttimizzaAmpiezzaLente(dw0, up0);
	//vector<double*> amp = { &up1->Ampiezza, &dw1->Ampiezza };
	//vector<double*> quo = { &up1->Quota, &dw1->Quota };
	//current.OttimizzaParametri(amp,quo);
	current.OttimizzaPosizioneSensore();
	current.Gnuplotta("double.pdf");
	//current.OttimizzaPosizioneLente(dw1, up1);
	inf.close();
	sup.close();
	inf.open(INPUT_DIR + string("downPoints.dat"));
	sup.open(INPUT_DIR + string("upPoints.dat"));

	Punti* dw2 = current.NuovoPunti(inf, 132, DIMENSIONE_SENSORE, AIR, AIR, PLEXIGLASS, PLEXIGLASS);
	Punti* up2 = current.NuovoPunti(sup, 132, DIMENSIONE_SENSORE, PLEXIGLASS, PLEXIGLASS, AIR, AIR);
	current.Sensore.Quota += 10;
	current.OttimizzaPosizioneSensore();
	current.OttimizzaAmpiezzaSensore();
	current.OttimizzaPosizioneLente(dw2, up2);
	current.OttimizzaPosizioneSensore();
	current.OttimizzaAmpiezzaSensore();
	current.Sensore.Quota += 5;
	current.Gnuplotta("triple.pdf");
	inf.close();
	sup.close();

	cout << "sensore at " << current.Sensore.Quota << endl;
	cout<<endl<<"... COMPLETED!.."<<endl;
	}
