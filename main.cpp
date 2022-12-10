#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include "lente.hpp"
#define num_raggi 20
#define ORD 2
#define SEMI_CAMPO 400
#define SEMI_SENSORE 10
#define ALTEZZA_CAMERA 1000
#define MAX_LOOP 600000
using namespace std;

// misure in mm
void Gnuplotta(Sistema a);

int main(){
	Sistema tele= Sistema({1.5},SEMI_CAMPO,ALTEZZA_CAMERA,SEMI_SENSORE,ORD);
	//srand(time(NULL));
	cout<<"...PROCESSING..."<<endl;
	for(int i=0;i<MAX_LOOP;++i){
		tele.Update(tele.Campo*rand()/RAND_MAX);
		//std::cout<<tele.Update(tele.Campo*rand()/RAND_MAX)<<std::endl;
		}
	cout<<"... COMPLETED!.."<<endl;
	
	Gnuplotta(tele);
    //DEBUG
    /*
    cout<<"num di lenti: "<<tele.lente.size()<<endl;
    for(int i=0;i<tele.lente[0].Inf.Q.size();i++){
    	cout<<"coeff inf "<<i<<": "<<tele.lente[0].Inf.Q[i]<<endl;
		}
	for(int i=0;i<tele.lente[0].Sup.Q.size();i++){
    	cout<<"coeff sup "<<i<<": "<<tele.lente[0].Sup.Q[i]<<endl;
		}
	*/
	}
void Gnuplotta(Sistema tele){
	for(int i=0; i < num_raggi; i++){
		ofstream fpt ("dati/"+to_string(i)+".dat");
		if ((fpt.is_open()) == false){
        	printf("Error! opening file");
        	exit(1);
    		}
   		Raggio ray = Raggio(fpt,-tele.Campo+i*2*tele.Campo/num_raggi,0,0.0);
		for(int i=0;i<tele.lente.size();i++){
			Raggio a = tele.lente[i].Out(fpt,ray);
			ray=a;
			}
		double x=tele.Sensore.Intersect(ray);
		ray= Raggio(fpt,x,tele.Sensore(x),0);
		fpt.close();
		}
	ofstream fpt ("data.gp");
	if ((fpt.is_open()) == false){
        printf("Error! opening file");
        exit(1);
    	}
    fpt<<("set size ratio -1\nset terminal pdf\nset output 'plot.pdf'\nset nokey\n");
    fpt<<("plot ["+to_string(-tele.Campo)+":"+to_string(tele.Campo)+"] [0:"+to_string(tele.Sensore(0))+"] ").c_str();
    tele.Log(fpt);
    for(int i=0; i < num_raggi; i++){
    	fpt<<"'dati/"+to_string(i)+".dat' u 1:2 with lines";
    	if (i+1<num_raggi) fpt<<", ";
    	}
    fpt.close();
    system("gnuplot -p data.gp");
    }
