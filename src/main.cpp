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
	
	Arco* sott = current.NuovoArco(0, 35, -130, AIR, AIR, PLEXIGLASS, PLEXIGLASS);
	Arco* sopr = current.NuovoArco(0, 35, 130, PLEXIGLASS, PLEXIGLASS, AIR, AIR);
	//current.OttimizzaElemento(sopr);
	//current.OttimizzaElemento(sott);
	current.Gnuplotta("arcos.pdf");
	current.RimuoviLente(sopr, sott);

	Punti* dw0 = current.NuovoPunti(inf, 0, 35, AIR, AIR,				  PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up0 = current.NuovoPunti(sup, 0, 35, PLEXIGLASS_R, PLEXIGLASS_B,					AIR, AIR);
	inf.close();
	sup.close();
	current.OttimizzaPosizioneSensore();
	current.Gnuplotta("single.pdf");
	
	inf.open(INPUT_DIR + string("downPoints.dat"));
	sup.open(INPUT_DIR + string("upPoints.dat"));

	Punti* dw1 = current.NuovoPunti(inf, 120.5, 4.0, AIR, AIR, PLEXIGLASS_R, PLEXIGLASS_B);
	Punti* up1 = current.NuovoPunti(sup, 120.5, 4.0, PLEXIGLASS_R, PLEXIGLASS_B, AIR, AIR);
	current.OttimizzaPosizioneLente(dw1, up1);
	current.OttimizzaPosizioneSensore();
	current.Gnuplotta("double.pdf");
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
	current.RimuoviLente(dw1, up1);/*
	current.OttimizzaAmpiezzaSensore();
	current.OttimizzaPosizioneSensore();
	current.OttimizzaAmpiezzaSensore();
	*/
	current.Gnuplotta("bis.pdf");
	inf.close();
	sup.close();

	cout << "sensore at " << current.Sensore.Quota << endl;
	cout<<endl<<"... COMPLETED!.."<<endl;
	}
