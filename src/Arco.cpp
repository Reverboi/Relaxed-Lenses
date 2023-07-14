#include "Arco.hpp"
namespace RelaxedLenses {
	double Arco :: operator()(double x) const {
		double result = Quota;
		if (R != 0.0) result += sign(R) * (sqrt((1.0 / (R * R)) - x * x) - sqrt((1.0 / (R * R)) - Ampiezza * Ampiezza));
		return result;
	}

	double Arco::Derivata(double x) const {
		double result = 0.0;
		if (R != 0.0) result += -x * sign(R) / (sqrt((1.0 / (R * R)) - x * x));
		return result;
	}

	void Arco::Log(std::ofstream& fpt) const {
		if (fpt.is_open()) {
			fpt << Quota << " + ";
			if (R != 0.0) fpt << "sgn(" << R << ")*(sqrt(1/(" << R * R << ") - x*x) - sqrt(1/(" << R * R << ") - " << Ampiezza * Ampiezza << ")), ";
			else       fpt << "0, ";
		}
	}

	void Arco::Deform(double x) { R += x; }

	int sign(double f) {
		if (f > 0) return 1;
		if (f == 0.0) return 0;
		else return -1;
	}
}