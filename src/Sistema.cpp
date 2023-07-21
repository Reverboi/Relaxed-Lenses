#include "Sistema.hpp"
namespace RelaxedLenses {
    Sistema::Sistema(double altSen, double dimSen, double camp) :  Campo(camp), Sensore(*NuovoArco(altSen, dimSen, 1, 1, 1, 1)) {}
    
    void Sistema::OttimizzaElemento(Curva* elem) {
        double scarto;
        elem->Deform(e);
        scarto = GScore();
        elem->Deform(-2 * e);
        scarto -= GScore();
        double deriv_inf = scarto / (2 * e);
        elem->Deform(e + deriv_inf * eps);
    }

    Raggio Sistema::Out_d(Raggio I) const {
        for (int i = 0; i < Elemento.size(); i++) {
            I = Elemento[i]->Out_d(I);
        }
        return I;
    }

    Raggio Sistema::Out_d(std::ofstream& fpt, Raggio I) const { // log version
        for (int i = 0; i < Elemento.size(); i++) {
            I = Elemento.at(i)->Out_d(fpt, I);
        }
        return I;
    }

    Raggio Sistema::Out_f(Raggio I) const {
        for (int i = 0; i < Elemento.size(); i++) {
            I = Elemento.at(i)->Out_f(I);
        }
        return I;
    }

    Raggio Sistema::Out_f(std::ofstream& fpt, Raggio I) const { // log version
        for (int i = 0; i < Elemento.size(); i++) {
            I = Elemento.at(i)->Out_f(fpt, I);
        }
        return I;
    }

    double Sistema::Score_d(const double x) const {
        Raggio in = Raggio(x, -Sensore.Quota / 2, 0);
        double target = -x * Sensore.Ampiezza / Campo;
        Raggio out = Out_d(in);
        if (out.X != out.X) return (Campo + Sensore.Ampiezza);
        return out.X - target;
    }

    double Sistema::Score_f(const double x)  const {
        Raggio in = Raggio(x, -Sensore.Quota / 2, 0);
        double target = -x * Sensore.Ampiezza / Campo;
        Raggio out = Out_f(in);
        double hit = out.X + (out.Y - Sensore.Quota) * tan(out.A);
        if (out.X != out.X) return (Campo + Sensore.Ampiezza);
        return out.X - target;
    }

    double Sistema::GScore() const {
        double res = 0.0;
        int G = RISOLUZIONE_G_SCORE;
        for (int i = 0; i < G; i++) {
            double  x = -Campo + i * Campo / G;
            double s1 = Score_d(x);
            double s2 = Score_f(x);
            res -= s1 * s1 + s2 * s2;
        }
        return res / (2 * G * Sensore.Ampiezza * Sensore.Ampiezza); // normalizzato sulla dimensione del sensore
    }

    void Sistema::Gnuplotta(std::string destination) const {
        std::ofstream sens(OUTPUT_DIR + std::string("sensore.dat"));
        sens << -Sensore.Ampiezza << " " << Sensore.Quota << "\n";
        sens << Sensore.Ampiezza << " " << Sensore.Quota;
        sens.close();

        for (int i = 1; i < num_raggi; i++) {		//creo i file per i singoli raggi
            std::ofstream fpt(OUTPUT_DIR + std::to_string(i) + std::string("_d.dat"));   //d
            if ((fpt.is_open()) == false) {
                printf("Error! opening file");
                exit(1);
            }
            Raggio ray = Raggio(fpt, -Campo + i * 2 * Campo / num_raggi, -Sensore.Quota / 2, 0.0);
            for (int i = 0; i < Elemento.size(); i++) {
                Raggio a = Elemento[i]->Out_d(fpt, ray);
                ray = a;
            }
            fpt << ray.X + (ray.Y - Sensore.Quota) * tan(ray.A) << " " << Sensore.Quota;
            fpt.close();

            fpt = std::ofstream(OUTPUT_DIR + std::to_string(i) + std::string("_f.dat"));     //f
            if ((fpt.is_open()) == false) {
                printf("Error! opening file");
                exit(1);
            }
            ray = Raggio(fpt, -Campo + i * 2 * Campo / num_raggi, -Sensore.Quota / 2, 0.0);
            for (int i = 0; i < Elemento.size(); i++) {
                Raggio a = Elemento[i]->Out_f(fpt, ray);
                ray = a;
            }
            fpt << ray.X + (ray.Y - Sensore.Quota) * tan(ray.A) << " " << Sensore.Quota;
            fpt.close();
        }
        std::ofstream fpt(OUTPUT_DIR + std::string("data.gp"));
        if ((fpt.is_open()) == false) {
            printf("Error! opening file");
            exit(1);
        }

        fpt << "set terminal pdf\nset output '" << PLOT_DIR << destination << "'\nset nokey\n";
        fpt << ("set size ratio -1\n");
        fpt << "set xlabel " << '"' << "Punteggio globale: " << GScore() << '"' << "\n";
        fpt << "unset xtics\n";

        fpt << "plot [-" << Campo << ":" << Campo << "] [-20:" << Sensore.Quota + 20 << "] ";

        Log(fpt);

        for (int i = 1; i < num_raggi; i++) {
            fpt << "'" << OUTPUT_DIR << i << "_d.dat' u 1:2 with lines lt rgb " << '"' << "orange" << '"';
            fpt << ", ";
        }
        fpt << " '" << OUTPUT_DIR << "sensore.dat" << "' u 1:2 with lines lt rgb " << '"' << "green" << '"';

        fpt << "\nplot [-" << Campo << ":" << Campo << "] [-20:" << Sensore.Quota + 20 << "] ";

        Log(fpt);

        for (int i = 1; i < num_raggi; i++) {
            fpt << "'" << OUTPUT_DIR << i << "_f.dat' u 1:2 with lines lt rgb " << '"' << "blue" << '"';
            /*if (i+1<num_raggi)*/ fpt << ", ";
        }
        fpt << " '" << OUTPUT_DIR << "sensore.dat" << "' u 1:2 with lines lt rgb " << '"' << "green" << '"';

        fpt << "\nset xtics\n";
        fpt << "set xlabel " << '"' << "Campo inquadrato (mm)" << '"' << "\n";
        fpt << "set ylabel " << '"' << "errore offset raggio (mm)" << '"' << "\n";
        fpt << ("\nset size noratio\n");
        fpt << "plot [:][-0.3:0.3] '" << OUTPUT_DIR << "scores.dat' u 1:2 with lines lt rgb" << '"' << "orange" << '"'
            << ",'" << OUTPUT_DIR << "scores.dat' u 1:3 with lines lt rgb" << '"' << "blue" << '"' << "\n";
        fpt << "set ylabel " << '"' << "errore offset raggio (log10-scale) " << '"' << "\n";
        fpt << "plot [:][:] '" << OUTPUT_DIR << "scores.dat' u 1:4 with lines\n";
        fpt.close();
        std::ofstream fpy(OUTPUT_DIR + std::string("scores.dat"));

        if ((fpy.is_open()) == false) {
            printf("Error! opening file");
            exit(1);
        }
        int pti = 100;
        for (int i = 1; i < pti; i++) {
            double x = -Campo + i * 2 * Campo / pti;
            double t = Score_d(x);
            double s = Score_f(x);
            fpy << x << ' ' << t << ' ' << s << ' ' << log10(sqrt(t * t + s * s)) << std::endl;
        }
        fpy.close();
        system((std::string("gnuplot -p ") + OUTPUT_DIR + std::string("data.gp")).c_str()); // non funzionerà su windows forse
    }

    void Sistema::OttimizzaPosizioneLente(Curva* a, Curva* b) {
        std::vector<double*> pino = { &(a->Quota), &(b->Quota) };
        OttimizzaParametriParalleli(pino);
    }

    void Sistema::OttimizzaAmpiezzaLente(Curva* a, Curva* b) {
        std::vector<double*> pino = { &(a->Ampiezza), &(b->Ampiezza) };
        OttimizzaParametriParalleli(pino);
    }

    void Sistema::OttimizzaPosizioneSensore() { OttimizzaParametro(Sensore.Quota); }

    void Sistema::OttimizzaAmpiezzaSensore() { OttimizzaParametro(Sensore.Ampiezza); }

    void Sistema::InserisciElemento(Curva* q) {
        Elemento.push_back(q);
        std::sort(Elemento.begin(), Elemento.end(), [](const Curva* a, const Curva* b) {return a->operator()(0) < b->operator()(0); });
    }

    void Sistema::Log(std::ofstream& fpt) const {
        for (int i = 0; i < Elemento.size(); i++) {
            Elemento[i]->Log(fpt);
        }
    }

    Sistema :: ~Sistema() {
        for (int i = 0; i < Elemento.size(); i++) delete Elemento[i];
    }

    Arco* Sistema::NuovoArco(double quota, double amp, double rag, double r1, double b1, double r2, double b2) {
        Arco* elem = new Arco(quota, amp, rag, r1, b1, r2, b2);
        Elemento.push_back(elem);
        std::sort(Elemento.begin(), Elemento.end(), [](const Curva* a, const Curva* b) {return a->operator()(0) < b->operator()(0); });
        return elem;
    }

    Arco* Sistema::NuovoArco(double quota, double amp, double r1, double b1, double r2, double b2) {
        Arco* elem = new Arco(quota, amp, r1, b1, r2, b2);
        Elemento.push_back(elem);
        std::sort(Elemento.begin(), Elemento.end(), [](const Curva* a, const Curva* b) {return a->operator()(0) < b->operator()(0); });
        return elem;
    }

    Polinomio* Sistema::NuovoPolinomio(std::ifstream& inp, double r1, double b1, double r2, double b2) {
        Polinomio* elem = new Polinomio(inp, r1, b1, r2, b2);
        Elemento.push_back(elem);
        std::sort(Elemento.begin(), Elemento.end(), [](const Curva* a, const Curva* b) {return a->operator()(0) < b->operator()(0); });
        return elem;
    }

    Punti* Sistema::NuovoPunti(std::ifstream& inp, double quota, double ampiezza, double r1, double b1, double r2, double b2) {
        Punti* elem = new Punti(inp, quota, ampiezza, r1, b1, r2, b2);
        Elemento.push_back(elem);
        std::sort(Elemento.begin(), Elemento.end(), [](const Curva* a, const Curva* b) {return a->operator()(0) < b->operator()(0); });
        return elem;
    }

    void Sistema::OttimizzaParametri(double& s, double& v) {
        double* p[2] = { &s, &v };
        double d[2];
        for (int j = 0; j < 20; j++) {
            double grad = 0;
            for (int i = 0; i < 2; i++) {

                *p[i] += e;
                double fy = GScore();
                *p[i] -= 2 * e;
                double by = GScore();
                *p[i] += e;

                d[i] = (fy - by) / (2 * e);
                grad += d[i] * d[i];
            }
            if (grad <= TRESH * 2) {
                std::cout << "broke at : " <<j<< std::endl;
                break;
            }
            for (int i = 0; i < 2; i++) *p[i] += d[i] * eps;
        }
    }

    void Sistema::OttimizzaParametriParalleli(std::vector<double*> a) {
        double fx, bx;
        double cx = GScore();
        double crawl = Sensore.Quota / 2.0;
        for (int i = 0; (i < 24) && (crawl * crawl >= e); i++) {
            for (int j=0; j<a.size();j++) *a[j] += crawl;
            fx = GScore();
            if (fx > cx) {
                cx = fx;
                continue;
            }
            for (int j=0; j<a.size();j++) *a[j] -= 2*crawl;
            bx = GScore();
            if (bx > cx) {
                cx = bx;
                crawl = -crawl;
                continue;
            }
            else {
                for (int j = 0; j < a.size(); j++) *a[j] += crawl;
                crawl = crawl / 2.0;
            }
        }
    }

    void Sistema::OttimizzaParametro(double& a) {
        std::vector<double*> pipo = { &a };
        OttimizzaParametriParalleli(pipo);
    }

    void Sistema::OttimizzaParametri(std::vector<double*> c, std::vector<double*> b) {
        double d[2];
        std::vector<std::vector<double*>> a = { c, b };
        for (int j = 0; j < 200; j++) {
            double grad = 0;
            for (int i = 0; i < a.size(); i++) {

                for (int k = 0; k < a[i].size(); k++) *a[i][k] += e;
                double fy = GScore();
                for (int k = 0; k < a[i].size(); k++) *a[i][k] -= 2 * e;
                double by = GScore();
                for (int k = 0; k < a[i].size(); k++) *a[i][k] += e;

                d[i] = (fy - by) / (2 * e);
                grad += d[i] * d[i];
            }
            if (grad <= TRESH * 2) {
                std::cout << "broke at : " << j << "with grad = " << grad << std::endl;
                break;
            }
            for (int i = 0; i < a.size(); i++) {
                for (int k = 0; k < a[i].size(); k++) *a[i][k] += d[i] * eps;
            }
        }
    }
}