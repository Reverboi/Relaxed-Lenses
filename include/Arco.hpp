#include "Curva.hpp"

struct Arco : Curva {
	double R;              // 1/raggio di curvatura
	Arco (double quota, double amp, double rag, double r1, double r2, double b1, double b2) : R(1.0/rag), Curva(quota, amp, r1,r2,b1,b2) {};
	Arco (double quota, double amp, double r1, double r2, double b1, double b2) : Curva(quota, amp, r1, r2, b1, b2), R (0.0) {}  //  Siccome non posso passargli Infinity...In realtà potrei
	double operator () (double x) const override;
	double Derivata(double x) const override;
	void Log(std::ofstream& fpt) const override;
	void Deform(double x) override;
	//Curva* Duplica() final {return new Arco(*this);};
    };

int sign(double f);
