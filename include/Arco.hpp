#include "Curva.hpp"

struct Arco : Curva {
	double R;              // 1/raggio di curvatura
	Arco (double quota, double amp, double rag) : R(1.0/rag), Curva(quota, amp) {};
	Arco (double quota, double amp) : Curva(quota, amp), R (0.0) {}  //  Siccome non posso passargli Infinity...
	double operator () (double x) const override;
	double Derivata(double x) const override;
	void Log(std::ofstream& fpt) const override;
	//Curva* Duplica() final {return new Arco(*this);};
    };

int sign(double f);
