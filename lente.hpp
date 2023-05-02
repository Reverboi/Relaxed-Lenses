struct Raggio{//tutti i raggi viaggiano verso l'alto
	double X,Y,A;
	//double W; //frequenza
	void Log(std::ofstream& fpt);
	Raggio(double x, double y, double a) : X(x),Y(y),A(a) {};
	Raggio(std::ofstream& fpt, double x, double y, double a) : X(x),Y(y),A(a) {Log(fpt);}
	};
	
struct Curva{
	std::vector<double> Q; // Coefficienti di Taylor
	Curva (std::vector<double> q, double amp, double rag) : Q{q}, Ampiezza(amp) {};
	void Log(std::ofstream& fpt);
	double Ampiezza;
	Curva (double quota, double amp, int ord) : Ampiezza(amp){ //crea una retta orizzontale con ord coeff a zero
		if(ord<0) exit(0);
		Q.reserve(ord);
		Q.push_back(quota);
		for(int i=1;i<ord;i++){
			Q.push_back(0.0);
			}
		}
	Curva (double quota, double amp, double rag, int ord) : Ampiezza(amp){ //crea una retta orizzontale con ord coeff a zero
		if(ord<0) exit(0);
		Q.reserve(ord);
		Q.push_back(quota);
		//std::cout<<"init"<<std::endl;
		double c[6]={0,-1,-3,-45,-1575,-99225};
		for(int i=1;i<ord;i++){
			if(i<6){
				Q.push_back(c[i]*pow(4*rag,-(i*2-1))*Ampiezza);
				//std::cout<<Q[i]<<std::endl;
				}
			else
			Q.push_back(0.0);
			}
		Q[0]=quota;
		std::cout<<Q.size()<<std::endl;
		}
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
	Lente (std::vector<double> inf, double ri, std::vector<double> sup, double rs, double nd, double nf, double amp) : Inf(inf,ri,amp), Sup(sup,rs,amp), N_d(nd), N_f(nf) {};
	Lente(Curva inf, Curva sup, double nd, double nf): Inf(inf), Sup(sup), N_d(nd), N_f(nf) {};//afraid
	};

struct Sistema{
	std::vector<Lente> lente;
	Curva Sensore;
	double AltezzaSensore;
	double DimensioneSensore;
	double Campo; //Dimensione massima dell'oggetto inquadrato
	Raggio Out_d (Raggio in);
	Raggio Out_f (Raggio in);
	void Log(std::ofstream& fpt);
	Raggio Out_d (std::ofstream &fpt, Raggio in);
	Raggio Out_f (std::ofstream &fpt, Raggio in);
        Sistema(std::vector<Lente> bingo, double campo, double ps, double ds) : Sensore (ps,ds,0), AltezzaSensore(ps), DimensioneSensore(ds), Campo(campo), lente(bingo){};
	Sistema(std::vector<double*> IndiciRifrazione, double campo, double ps, double ds, int ord) : Sensore(ps,ds,1), AltezzaSensore(ps), DimensioneSensore(ds), Campo(campo) {
		int n = IndiciRifrazione.size();
		lente.reserve(n);
		for(int i = 0; i < n; i++){
		    lente.push_back( 
			  Lente(Curva(ps*(i+1)/(n+1)-50,campo,-1000000.0,ord),
			        Curva(ps*(i+1)/(n+1)+50,campo,+1000000.0,ord),
			        IndiciRifrazione[i][0],
			        IndiciRifrazione[i][1])
			  );
		    }
	      }
	};

void Gnuplotta(Sistema&);

void GlobalUpdate(Sistema& D,Sistema& R, double eps);	//gradiente
void GlobalUpdate(Sistema& D, Sistema& R, int i, int j, double eps);//componente scelta

double Score_d(Sistema&, double x);
double Score_f(Sistema&, double x);
double GScore(Sistema&);
double Snell(double angolo, double index);
int sign(double f);
