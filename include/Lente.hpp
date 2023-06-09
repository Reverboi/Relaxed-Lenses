#include "Curva.hpp"

#include <vector>
struct Lente{
	Curva& Inf;
	Curva& Sup;
	double N_d;   // Indice di rifrazione (589.2 nm)
	double N_f;   // Indice di rifrazione (468.1 nm)
	Raggio Out_d (std::ofstream &fpt, const Raggio& I) const;
	Raggio Out_d (const Raggio& I) const;
	Raggio Out_f (std::ofstream &fpt, const Raggio& I) const;
	Raggio Out_f (const Raggio& I) const;
	void Log(std::ofstream &fpt) const;
	Lente(Curva& inf, Curva& sup, double nd, double nf): Inf(inf), Sup(sup), N_d(nd), N_f(nf) {};
	//Lente& operator=(Lente&& bob);
	//Lente(const Lente& source): Inf(source.Inf.Duplica()), Sup(source.Sup.Duplica()), N_d(source.N_d), N_f(source.N_f){};
	//~Lente();     // frees the curves
	};

double Snell(double angolo, double index);
