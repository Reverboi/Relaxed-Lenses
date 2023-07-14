#include "Curva.hpp"
#include <vector>
namespace RelaxedLenses {
    struct Punto {
        double x;
        double y;
        Punto(double a, double b) : x(a), y(b) {};
    };

    struct Punti : Curva {
        std::vector<Punto> P;
        Punti(double quota, double amp, std::vector<Punto> q, double r1, double b1, double r2, double b2) : Curva(quota, amp, r1, b1, r2, b2), P{ q } {};
        Punti(double quota, double amp, std::vector<Punto> q, double i1, double i2) : Curva(quota, amp, i1, i2), P{ q } {};  // pobably not going to use this one
        Punti(std::ifstream& inp, double quota, double ampiezza, double r1, double b1, double r2, double b2);   // load info from file
        double operator () (double x) const override;
        double Derivata(double x) const override;
        void Log(std::ofstream& fpt) const override;
        //Curva* Duplica() final {return new Polinomio(*this);};
    };

    double ArcFrom3Points(Punto a0, Punto b0, Punto c0, double xx);
    double ArcDerivataFrom3Points(Punto a0, Punto b0, Punto c0, double xx);
}