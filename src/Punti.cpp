#include "Punti.hpp"
#include <string>
#include <algorithm>

namespace RelaxedLenses {
    double Punti :: operator()(double u) const {
        if (u != u) return NAN;
        if (u * u > Ampiezza * Ampiezza)
            return NAN;
        u = u / Ampiezza;
        if (u < 0)  u = -u;
        int i = u * (P.size() - 1);
        double v;
        if (i <= 0) {
            Punto helper = Punto(-P[1].x, P[1].y);
            v = ArcFrom3Points(helper, P[0], P[1], u);
        }
        else if (i >= P.size() - 1) {
            v = ArcFrom3Points(P[P.size() - 3], P[P.size() - 2], P[P.size() - 1], u);
        }
        else {
            v = ArcFrom3Points(P[i - 1], P[i], P[i + 1], u);
        }
        return v * Ampiezza + Quota;
    }

    double ArcFrom3Points(Punto a0, Punto b0, Punto c0, double xx)
    {
        Punto a = a0;
        Punto b = b0;
        Punto c = c0;

        if (a.y == b.y)    // Switch points if two consecutives have the same y
        {                   // causes begin angle and end angle to be incorrect
            Punto f = c;
            c = b;
            b = f;
        }
        else if (b.y == c.y)
        {
            Punto f = a;
            a = b;
            b = f;
        }
        Punto d = Punto((a.x + b.x) / 2.0, (a.y + b.y) / 2.0); // middle point between a and b
        Punto e = Punto((c.x + b.x) / 2.0, (c.y + b.y) / 2.0); // middle point between b and c
        double s = -(b.x - a.x) / (b.y - a.y);  // normal slope to segment ab
        double t = -(c.x - b.x) / (c.y - b.y);  // normal slope to segment bc
        Punto origin = Punto(0, 0);
        origin.x = (e.y - d.y - e.x * t + d.x * s) / (s - t);
        origin.y = d.y + s * (origin.x - d.x);

        double radius = sqrt((origin.x - a.x) * (origin.x - a.x) + (origin.y - a.y) * (origin.y - a.y));
        int segno = 1;
        if (b0.y < a0.y + (c0.y - a0.y) * (b0.x - a0.x) / (c0.x - a0.x)) segno = -1;
        return sqrt(radius * radius - (xx - origin.x) * (xx - origin.x)) * segno + origin.y;
    }

    double ArcDerivataFrom3Points(Punto a0, Punto b0, Punto c0, double xx)
    {
        Punto a = a0;
        Punto b = b0;
        Punto c = c0;

        if (a.y == b.y)    // Switch points if two consecutives have the same y
        {                   // causes begin angle and end angle to be incorrect
            Punto f = c;
            c = b;
            b = f;
        }
        else if (b.y == c.y)
        {
            Punto f = a;
            a = b;
            b = f;
        }
        Punto d = Punto((a.x + b.x) / 2.0, (a.y + b.y) / 2.0); // middle point between a and b
        Punto e = Punto((c.x + b.x) / 2.0, (c.y + b.y) / 2.0); // middle point between b and c
        double s = -(b.x - a.x) / (b.y - a.y);  // normal slope to segment ab
        double t = -(c.x - b.x) / (c.y - b.y);  // normal slope to segment bc
        Punto origin = Punto(0, 0);
        origin.x = (e.y - d.y - e.x * t + d.x * s) / (s - t);
        origin.y = d.y + s * (origin.x - d.x);

        double radius = sqrt((origin.x - a.x) * (origin.x - a.x) + (origin.y - a.y) * (origin.y - a.y));
        int segno = 1;
        if (b0.y < a0.y + (c0.y - a0.y) * (b0.x - a0.x) / (c0.x - a0.x)) segno = -1;
        return (origin.x - xx) / sqrt(radius * radius - (xx - origin.x) * (xx - origin.x)) * segno;
    }

    double Punti::Derivata(double u) const {
        if (u != u) return NAN;
        if (u * u > Ampiezza * Ampiezza) return NAN;
        u = u / Ampiezza;
        int segno = 1;
        if (u < 0) {
            segno = -1;
            u = -u;
        }
        int i = u * (P.size() - 1);
        double result;
        if (i <= 0) {
            Punto helper = Punto(-P[1].x, P[1].y);
            result = ArcDerivataFrom3Points(helper, P[0], P[1], u) * segno;
        }
        else if (i >= P.size() - 1) {
            result = ArcDerivataFrom3Points(P[P.size() - 3], P[P.size() - 2], P[P.size() - 1], u) * segno;
        }
        else {
            result = ArcDerivataFrom3Points(P[i - 1], P[i], P[i + 1], u) * segno;
        }
        return result;
    }


    Punti::Punti(std::ifstream& inp, double quota, double ampiezza, double r1, double b1, double r2, double b2) : Curva(quota, ampiezza, r1, b1, r2, b2) {
        std::string line;
        while (std::getline(inp, line)) {  // read each line from the file
            char* pin;
            double x = std::strtod(line.c_str(), &pin);
            double y = std::strtod(pin + 1, &pin);
            P.push_back(Punto(x, y));
        }
        if (P.size() <= 3)  throw;
        std::sort(P.begin(), P.end(), [](const Punto a, const Punto b) {return a.x < b.x; });
        if (P[0].x < 0.0) {
            for (int i = 0; i < P.size(); i++) {
                P[i].x *= -1;
            }
        }
        std::sort(P.begin(), P.end(), [](const Punto a, const Punto b) {return a.x < b.x; });
        if (P.begin()->x != 0.0) throw;
        double r = P.rbegin()->x;
        if (r == 1.0) return;
        for (int i = 1; i < P.size(); i++) { //normalizzazione
            P[i].x /= r;
            P[i].y /= r;
        }
        P[0].y /= r;
    }

    void Punti::Log(std::ofstream& fpt) const {
        if (fpt.is_open()) {
            fpt << Quota << ", " << this->operator()(0) << ", ";
        }
    }
}