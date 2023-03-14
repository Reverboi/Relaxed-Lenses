struct Raggio{//tutti i raggi viaggiano verso l'alto
	double X,Y,A;
	void Log(std::ofstream& fpt);
	Raggio(double x, double y, double a) : X(x),Y(y),A(a) {};
	Raggio(std::ofstream& fpt, double x, double y, double a) : X(x),Y(y),A(a) {Log(fpt);}
	};
	
struct Curva{
	std::vector<double> Q; // Coefficienti di Fourier (solo coseni)
	Curva (std::vector<double> q,double amp) : Q{q}, Ampiezza(amp) {};
	void Log(std::ofstream& fpt);
	double Ampiezza;
	Curva (double quota, double amp, int ord) : Ampiezza(amp){ //crea una retta orizzontale con ord coeff a zero
		if(ord<0) exit(0);
		Q.reserve(ord+1);
		Q.push_back(quota);
		for(int i=0;i<ord;i++){
			Q.push_back(0.0);
			}
		}
	double operator()(double x) const;
	double Angolo(double x);
	double Derivata(double x);
	double Intersect(const Raggio& in);
	};

struct Lente{
	Curva Inf;
	Curva Sup;
	double N;		  // Indice di rifrazione 
	Raggio Out (std::ofstream &fpt, const Raggio& I);
	Raggio Out (const Raggio& I);
	void Log(std::ofstream &fpt);
	Lente(std::vector<double> inf, std::vector<double> sup, double n, double amp) : Inf(inf,amp), Sup(sup,amp), N(n) {};
	Lente(Curva inf, Curva sup, double n): Inf(inf), Sup(sup), N(n) {};//afraid
	};

struct Sistema{
	std::vector<Lente> lente;
	Curva Sensore;
	double AltezzaSensore;
	double DimensioneSensore;
	double Campo; //Dimensione massima dell'oggetto inquadrato
	Raggio Out( Raggio in);
	void Log(std::ofstream& fpt);
	Raggio Out(std::ofstream &fpt, Raggio in);
	
	Sistema(std::vector<double> IndiciRifrazione, double campo, double ps, double ds, int ord) : Sensore(ps,ds,0), AltezzaSensore(ps), DimensioneSensore(ds), Campo(campo) {
		int n = IndiciRifrazione.size();
		lente.reserve(n);
		for(int i = 0; i < n; i++){
			//lente.push_back( Lente(Curva(ps*(2*(i+1)-1)/(2*n+1),ord),Curva(ps*(2*(i+1))/(2*n+1),ord),IndiciRifrazione[i]));
			lente.push_back( Lente(Curva(ps*(i+1)/(n+1)-25,campo,ord),Curva(ps*(i+1)/(n+1)+25,campo,ord),IndiciRifrazione[i]));
			}
		}
	};
void Gnuplotta(Sistema&);
double RandomUpdate(Sistema& len);
double GlobalUpdate(Sistema& D);
double GlobalUpdate(Sistema& D, int len, int ord);
double Score(Sistema&, double x);
double GScore(Sistema&);
double Snell(double angolo, double index);
