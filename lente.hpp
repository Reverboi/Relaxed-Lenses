struct Raggio{//tutti i raggi viaggiano verso l'alto
	double X,Y,A;
	void Log(std::ofstream& fpt) const;
	Raggio(double x, double y, double a) : X(x),Y(y),A(a) {};
	Raggio(std::ofstream& fpt, double x, double y, double a) : X(x),Y(y),A(a) {Log(fpt);}
	};
	
struct Curva{
        double Quota;          // altezza di riferimento del profilo della curva
	double Ampiezza;       // Semi-Apertura
	Curva (double quota, double amp) : Ampiezza(amp), Quota(quota) {};
	Curva(){};
	virtual void Log(std::ofstream& fpt) const = 0;      
	virtual double operator()(double x) const = 0;     
	virtual double Derivata(double x) const = 0;
	double Angolo(double x) const;
	double Intersect(const Raggio& in) const;
	//virtual Curva* Duplica() =0;
	};
	
struct Arco : Curva {
	double R;              // 1/raggio di curvatura
	Arco (double quota, double amp, double rag) : R(1.0/rag), Curva(quota, amp) {};
	Arco (double quota, double amp) : Curva(quota, amp), R (0.0) {}  //  Siccome non posso passargli Infinity...
	double operator () (double x) const override;
	double Derivata(double x) const override;
	void Log(std::ofstream& fpt) const override;
	//Curva* Duplica() final {return new Arco(*this);};
        };
        
struct Polinomio : Curva {
       std::vector<double> Q; // Coefficienti di polinomi pari a partire da Q[0]*(A^2-x^2) 
       Polinomio (double quota, double amp, std::vector<double> q) : Curva(quota, amp), Q{q} {};  // pobably not going to use this one
       Polinomio (std::ifstream& file);   // load info from file
       double operator () (double x) const override;
       double Derivata(double x) const override;
       void Log(std::ofstream& fpt) const override;
       //Curva* Duplica() final {return new Polinomio(*this);};
       };
       
struct Lente{
	Curva& Inf;
	Curva& Sup;
	double N_d;   // Indice di rifrazione (589.2 nm)
	double N_f;   // Indice di rifrazione (468.1 nm)
	Raggio Out_d (std::ofstream &fpt, const Raggio& I) const;
	Raggio Out_d (const Raggio& I) const;
	Raggio Out_f (std::ofstream &fpt, const Raggio& I) const;
	Raggio Out_f (const Raggio& I) const;
	void Log(std::ofstream &fpt) const;
	Lente(Curva& inf, Curva& sup, double nd, double nf): Inf(inf), Sup(sup), N_d(nd), N_f(nf) {};
	
	//Lente& operator=(Lente&& bob);
	//Lente(const Lente& source): Inf(source.Inf.Duplica()), Sup(source.Sup.Duplica()), N_d(source.N_d), N_f(source.N_f){};
	//~Lente();     // frees the curves
	};

struct Sistema{
	std::vector<Lente> lente;
	double AltezzaSensore;
	double DimensioneSensore; // metà della lunghezza del sensore
	double Campo;             // metà del campo inquadrato
	Raggio Out_d (Raggio in) const;
	Raggio Out_f (Raggio in) const;
	void Log(std::ofstream& fpt) const;
	Raggio Out_d (std::ofstream &fpt, Raggio in) const;
	Raggio Out_f (std::ofstream &fpt, Raggio in) const;
        Sistema(double altSen, double dimSen, double camp) : AltezzaSensore(altSen), DimensioneSensore(dimSen), Campo(camp) {};
        Sistema(const Sistema& source);
        void InserisciLente(const Lente&);
        void OttimizzaSensore();
        double Score_d(double x) const;
        double Score_f(double x) const;
        double GScore() const;
        void Gnuplotta(std::string destination) const;
	};

double Snell(double angolo, double index);
int sign(double f);
