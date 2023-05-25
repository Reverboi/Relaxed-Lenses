struct Raggio{//tutti i raggi viaggiano verso l'alto
	double X,Y,A;
	//double W; //frequenza
	void Log(std::ofstream& fpt);
	Raggio(double x, double y, double a) : X(x),Y(y),A(a) {};
	Raggio(std::ofstream& fpt, double x, double y, double a) : X(x),Y(y),A(a) {Log(fpt);}
	};
	
struct Curva{
        double Quota;          // altezza del profilo della curva ai suoi estremi
	std::vector<double> Q; // Coefficienti di polinomi pari a partire da Q[0]*(A^2-x^2)
	double R;              // 1/raggio di curvatura
	double Ampiezza;       // Semi-Apertura
	Curva (double quota, double amp, std::vector<double> q) : Ampiezza(amp), Q{q}, R(0.0) {};  // pobably not going to use this one
	Curva (double quota, double amp, double rag) : Ampiezza(amp), Quota(quota), R(1.0/rag) {};
	Curva (std::ifstream& file);   // load info from file
	void Log(std::ofstream& fpt);
	double operator()(double x) const;
	double Angolo(double x);
	double Derivata(double x);
	double Intersect(const Raggio& in);
	};

struct Lente{
	Curva Inf;
	Curva Sup;
	double N_d;   // Indice di rifrazione (589.2 nm)
	double N_f;   // Indice di rifrazione (468.1 nm)
	Raggio Out_d (std::ofstream &fpt, const Raggio& I);
	Raggio Out_d (const Raggio& I);
	Raggio Out_f (std::ofstream &fpt, const Raggio& I);
	Raggio Out_f (const Raggio& I);
	void Log(std::ofstream &fpt);
	Lente(Curva inf, Curva sup, double nd, double nf): Inf(inf), Sup(sup), N_d(nd), N_f(nf) {};  
	};

struct Sistema{
	std::vector<Lente> lente;
	double AltezzaSensore;
	double DimensioneSensore; // metà della lunghezza del sensore
	double Campo;             // metà del campo inquadrato
	Raggio Out_d (Raggio in);
	Raggio Out_f (Raggio in);
	void Log(std::ofstream& fpt);
	Raggio Out_d (std::ofstream &fpt, Raggio in);
	Raggio Out_f (std::ofstream &fpt, Raggio in);
        Sistema(double altSen, double dimSen, double camp) : AltezzaSensore(altSen), DimensioneSensore(dimSen), Campo(camp) {};
        
        void OttimizzaSensore();
        double Score_d(double x);
        double Score_f(double x);
        double GScore();
        void Gnuplotta(std::string destination);
	};

void GlobalUpdate(Sistema& D,Sistema& R, double eps);	//gradiente
void GlobalUpdate(Sistema& D, Sistema& R, int i, int j, double eps);//componente scelta


double Snell(double angolo, double index);
int sign(double f);
