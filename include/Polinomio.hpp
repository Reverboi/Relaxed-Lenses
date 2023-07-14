#include "Curva.hpp"
#include <vector>
namespace RelaxedLenses {
    struct Polinomio : Curva {
        std::vector<double> Q; // Coefficienti di polinomi pari a partire da Q[0]*(A^2-x^2) 
        Polinomio(double quota, double amp, std::vector<double> q, double r1, double b1, double r2, double b2) : Curva(quota, amp, r1, b1, r2, b2), Q{ q } {};
        Polinomio(double quota, double amp, std::vector<double> q, double i1, double i2) : Curva(quota, amp, i1, i2), Q{ q } {};  // pobably not going to use this one
        Polinomio(std::ifstream& inp, double r1, double b1, double r2, double b2);   // load info from file
        double operator () (double x) const override;
        double Derivata(double x) const override;
        void Log(std::ofstream& fpt) const override;
        //Curva* Duplica() final {return new Polinomio(*this);};
    };

    std::string Numline(std::string str);
}