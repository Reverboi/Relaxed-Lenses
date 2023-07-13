#include <Curva.hpp>
#include <string>
#include <vector>
#include <algorithm>
#define RISOLUZIONE_G_SCORE 64
#define num_raggi 16
#define e ldexp(1.0,-30)
#define eps ldexp(1.0,-20)
#define OUTPUT_DIR std::string("../data/output/")
#define PLOT_DIR std::string("../data/plot/")

struct Sistema{
	//std::vector<Lente> lente;
	std::vector<Curva*> Elemento;
	double AltezzaSensore;
	double DimensioneSensore; // metà della lunghezza del sensore
	double Campo;             // metà del campo inquadrato
	Raggio Out_d (Raggio in) const;
	Raggio Out_f (Raggio in) const;
	void Log(std::ofstream& fpt) const;
	Raggio Out_d (std::ofstream &fpt, Raggio in) const;
	Raggio Out_f (std::ofstream &fpt, Raggio in) const;
        Sistema(double altSen, double dimSen, double camp) : AltezzaSensore(altSen), DimensioneSensore(dimSen), Campo(camp) {};
        //Sistema(const Sistema& source);
        //void InserisciLente(const Lente&);
		void InserisciElemento( Curva* );
        void OttimizzaSensore();
        double Score_d(double x) const;
        double Score_f(double x) const;
        double GScore() const;
        void Gnuplotta(std::string destination) const;
        void OttimizzaElemento(int i);
		~Sistema();
};
