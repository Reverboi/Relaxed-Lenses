
#include "Curva.hpp"
#include "Polinomio.hpp"
#include "Arco.hpp"
#include "Punti.hpp"
#include <string>
#include <vector>
#include <algorithm>
#define RISOLUZIONE_STANDARD 16
#define e ldexp(1.0,-50)
#define eps ldexp(1.0,-16)
#define TRESH 10e-8
#define OUTPUT_DIR std::string("../data/output/")
#define PLOT_DIR std::string("../data/plot/")
namespace RelaxedLenses {
	struct Sistema {

		std::vector<std::vector<Raggio>> Data_d;
		std::vector<std::vector<Raggio>> Data_f;

		Raggio RaggioIniziale;
		Raggio RaggioFinale;
		int NumeroRaggi;

		std::vector<Curva*> Elemento;
		//double Sensore.Quota;
		Curva& Sensore; // metà della lunghezza del sensore
		double Campo;             // metà del campo inquadrato
		Raggio Out_d(Raggio in) const;
		Raggio Out_f(Raggio in) const;
		void Log(std::ofstream& fpt) const;
		Raggio Out_d(std::ofstream& fpt, Raggio in) const;
		Raggio Out_f(std::ofstream& fpt, Raggio in) const;
		Sistema(double altSen, double dimSen, double camp);
		
		//void Elabora(int res);
		void Elabora();

		void InserisciElemento(Curva*); //viola la regola del one new one delete
		void RimuoviCurva(Curva*);
		void RimuoviLente(Curva*, Curva*);
		void OttimizzaPosizioneSensore();
		double Score_d(double x) const;
		double Score_f(double x) const;
		double GScore();
		void Gnuplotta(std::string destination);
		
		double Scarto(std::vector<Raggio>&);
		double IntercettaAsseOttico(std::vector<Raggio>&);

		void OttimizzaParametri(double& s, double& v);
		void OttimizzaParametri(std::vector<double*>, std::vector<double*>);
		void OttimizzaParametriParalleli(std::vector<double*>);
		void OttimizzaParametro(double&);
		void OttimizzaElemento( Curva * );
		void OttimizzaPosizioneLente(Curva* a, Curva* b);
		void OttimizzaAmpiezzaLente(Curva* a, Curva* b);
		void OttimizzaAmpiezzaSensore();
		Arco* NuovoArco(double quota, double amp, double rag, double r1, double b1, double r2, double b2);
		Arco* NuovoArco(double quota, double amp, double r1, double b1, double r2, double b2);
		Polinomio* NuovoPolinomio(std::ifstream& inp, double r1, double r2, double b1, double b2);   
		Punti* NuovoPunti(std::ifstream& inp, double quota, double ampiezza, double r1, double r2, double b1, double b2);
		~Sistema();
	};
}