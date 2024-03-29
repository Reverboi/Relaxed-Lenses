#include "Curva.hpp"
#define e  ldexp(1.0,-100) // numero piccolo: 'e' sta per epsilon
#define MAX_LOOP 1000

namespace RelaxedLenses {

	double Curva::Angolo(double x) const { return atan(Derivata(x)); }

	double Curva::Intersect(const Raggio& in) const {
		if (in.A != in.A) return NAN; //total internal reflection
		if (in.Y > ((*this)(in.X))) {  // ...se il raggio è già oltre...
			return NAN;
		}
		double r = 1;
		double const xx = -sin(in.A);
		double const yy = cos(in.A);
		double x0, y0, x1 = in.X, y1 = in.Y;
		for (int i = 0; i < MAX_LOOP; i++) {
			if ((y1 - (*this)(x1)) * (y1 - (*this)(x1)) <= e * e) break;
			if (y1 > (*this)(x1)) r = r / 2;
			else {
				x0 = x1;
				y0 = y1;
			}
			x1 = x0 + r * xx;
			y1 = y0 + r * yy;
		}
		return x1;
	}

	Raggio Curva::Out_f(std::ofstream& fpt, const Raggio& I) const { //log version
		double x = Intersect(I);
		return Raggio(fpt, x, operator()(x), Snell(I.A - Angolo(x), IndiceBluSopra / IndiceBluSotto) + Angolo(x));
	}

	Raggio Curva::Out_f(const Raggio& I) const {
		double x = Intersect(I);
		return Raggio(x, operator()(x), Snell(I.A - Angolo(x), IndiceBluSopra / IndiceBluSotto) + Angolo(x));
	}

	Raggio Curva::Out_d(std::ofstream& fpt, const Raggio& I) const { //log version
		double x = Intersect(I);
		return Raggio(fpt, x, operator()(x), Snell(I.A - Angolo(x), IndiceRossoSopra / IndiceRossoSotto) + Angolo(x));
	}

	Raggio Curva::Out_d(const Raggio& I) const {
		double x = Intersect(I);
		return Raggio(x, operator()(x), Snell(I.A - Angolo(x), IndiceRossoSopra / IndiceRossoSotto) + Angolo(x));
	}

	double Snell(double angolo, double index) { //ritorna +- NAN se siamo in total internal reflection
		return asin(sin(angolo) / index);
	}
}