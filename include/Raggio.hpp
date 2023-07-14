#include <iostream>
#include <fstream>
#include <cmath>
namespace RelaxedLenses {
	struct Raggio {//tutti i raggi viaggiano verso l'alto
		double X, Y, A;
		void Log(std::ofstream& fpt) const;
		Raggio(double x, double y, double a) : X(x), Y(y), A(a) {};
		Raggio(std::ofstream& fpt, double x, double y, double a) : X(x), Y(y), A(a) { Log(fpt); }
	};
}