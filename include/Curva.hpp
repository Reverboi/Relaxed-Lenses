#pragma once

#include "Raggio.hpp"
struct Curva{
        double Quota;          // altezza di riferimento del profilo della curva
	double Ampiezza;       // Semi-Apertura
	Curva (double quota, double amp) : Ampiezza(amp), Quota(quota) {};
	Curva(){};
	virtual void Log(std::ofstream& fpt) const = 0 ;      
	virtual double operator()(double x)  const = 0 ;     
	virtual double Derivata (double x)   const = 0 ;
	double Angolo(double x) const;
	double Intersect(const Raggio& in) const;
	//virtual Curva* Duplica() =0;
	};
