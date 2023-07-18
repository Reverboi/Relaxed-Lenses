#pragma once

#include "Raggio.hpp"
namespace RelaxedLenses {
	struct Curva {
		double Quota;          // altezza di riferimento del profilo della curva
		double Ampiezza;       // Semi-Apertura
		double IndiceRossoSotto,// d = rosso
			IndiceRossoSopra,
			IndiceBluSotto,     // f = blu
			IndiceBluSopra;
		Curva(double quota, double amp, double r1, double b1, double r2, double b2) : Ampiezza(amp), Quota(quota), IndiceRossoSotto(r1), IndiceRossoSopra(r2), IndiceBluSotto(b1), IndiceBluSopra(b2) {};
		Curva(double quota, double amp, double i1, double i2) : Ampiezza(amp), Quota(quota), IndiceRossoSotto(i1), IndiceRossoSopra(i2), IndiceBluSotto(i1), IndiceBluSopra(i2) {};
		Curva() {};
		virtual void Log(std::ofstream& fpt) const = 0;
		virtual double operator()(double x)  const = 0;
		virtual double Derivata(double x)   const = 0;
		virtual void Deform(double x) {};		// necessario che sia reversibile;
		double Angolo(double x) const;
		double Intersect(const Raggio& in) const;
		Raggio Out_f(std::ofstream& fpt, const Raggio& I) const;
		Raggio Out_f(const Raggio& I) const;
		Raggio Out_d(std::ofstream& fpt, const Raggio& I) const;
		Raggio Out_d(const Raggio& I) const;
	};

	double Snell(double angolo, double index);
}