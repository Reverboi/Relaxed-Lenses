#include "Polinomio.hpp"
#include <string>
namespace RelaxedLenses {
    double Polinomio :: operator()(double x) const {
        double result = Quota;
        for (int i = 0; i < Q.size(); i++) {
            result += Q[i] * (pow(Ampiezza, 2 * (i + 1)) - pow(x, 2 * (i + 1))) / pow(Ampiezza, 2 * (i + 1));
        }
        return result;
    }

    double Polinomio::Derivata(double x) const {
        double result = 0.0;
        for (int i = 0; i < Q.size(); i++) {
            result -= Q[i] * (i + 1) * 2 * pow(x, i * 2 + 1) / pow(Ampiezza, 2 * (i + 1));
        }
        return result;
    }

    Polinomio::Polinomio(std::ifstream& inp, double r1, double b1, double r2, double b2) : Curva(0, 0, r1, b1, r2, b2) {
        std::string line;
        std::string numline;
        while (std::getline(inp, line)) {  // read each line from the file
            numline = Numline(line);
            char* pin;
            double num = std::strtod(numline.c_str(), &pin);
            std::string rest;
            switch (line[0]) {
            case 'A':
                Ampiezza = num;
                break;
            case 'H':
                Quota = num;
                break;
            case 'Q':
                Quota += num;
            flag:
                rest = numline.substr(pin - &(numline[0]), 10000);
                numline = Numline(rest);
                if (numline != "") {
                    num = std::strtod(numline.c_str(), &pin);
                    Q.push_back(num);
                    goto flag;
                }
                break;
            default: break;
            }
        }
    }

    void Polinomio::Log(std::ofstream& fpt) const {
        if (fpt.is_open()) {
            fpt << Quota;
            for (int i = 0; i < Q.size(); i++) {
                fpt << " + " << Q[i] << " * (" << Ampiezza << "**" << (i + 1) * 2 << " - x**" << (i + 1) * 2 << ") / " << Ampiezza << "**" << (i + 1) * 2;
            }
        }
        fpt << ", ";
    }

    std::string Numline(std::string str) {     // Removes all leading non-number char until it finds a number
        std::string numStr = "";
        for (int i = 0; i < str.length(); i++) {
            if (isdigit(str[i]) || str[i] == '.' || str[i] == '-' || str[i] == 'e') {
                numStr = str.substr(i, str.length() - i);
                break;
            }
        }
        return numStr;
    }
}