#include "Curva.hpp"
#include <vector>
struct Polinomio : Curva {
       std::vector<double> Q; // Coefficienti di polinomi pari a partire da Q[0]*(A^2-x^2) 
       Polinomio (double quota, double amp, std::vector<double> q) : Curva(quota, amp), Q{q} {};  // pobably not going to use this one
       Polinomio (std::ifstream& file);   // load info from file
       double operator () (double x) const override;
       double Derivata(double x) const override;
       void Log(std::ofstream& fpt) const override;
       //Curva* Duplica() final {return new Polinomio(*this);};
       };
       
std::string Numline(std::string str);
