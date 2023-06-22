#include "Lente.hpp"

double Snell( double angolo, double index ){ //ritorna +- NAN se siamo in total internal reflection
	return asin( sin( angolo ) / index );
	}

Raggio Lente :: Out_d (std::ofstream& fpt, const Raggio& I ) const { //log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(fpt, x, Inf(x), Snell(I.A-Inf.Angolo(x),N_d)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(fpt, x, Sup(x), Snell(M.A-Sup.Angolo(x), 1.0/N_d)+Sup.Angolo(x));
	}

Raggio Lente :: Out_d (const Raggio& I) const { //non log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(x, Inf(x), Snell(I.A-Inf.Angolo(x),N_d)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(x, Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_d)+Sup.Angolo(x));
	}

Raggio Lente :: Out_f (std::ofstream& fpt, const Raggio& I) const { //log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(fpt, x, Inf(x), Snell(I.A-Inf.Angolo(x),N_f)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(fpt,x, Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_f)+Sup.Angolo(x));
	}

Raggio Lente :: Out_f (const Raggio& I) const { //non log version
	double x=Inf.Intersect(I);
	Raggio M = Raggio(x, Inf(x), Snell(I.A-Inf.Angolo(x),N_f)+Inf.Angolo(x));
	x=Sup.Intersect(M);
	return Raggio(x, Sup(x),Snell(M.A-Sup.Angolo(x),1.0/N_f)+Sup.Angolo(x));
	}

void Lente :: Log(std::ofstream &fpt) const {
	Inf.Log(fpt);
	Sup.Log(fpt);
	}

/*
void GlobalUpdate(Sistema& D, Sistema& R, double eps){
	double sfx,sbx;
	Sistema fx = D;
	Sistema bx = D;
	R = Sistema(D);
	for(int i=0;i<D.lente.size();i++){
		for(int j=0;j<D.lente[i].Inf.Q.size();j++){
			sfx=fx.lente[i].Inf.Q[j];
			sbx=bx.lente[i].Inf.Q[j];
			fx.lente[i].Inf.Q[j] += e2;
			bx.lente[i].Inf.Q[j] -= e2;
			R.lente[i].Inf.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2*(j+1));
			fx.lente[i].Inf.Q[j] = sfx;
			bx.lente[i].Inf.Q[j] = sbx;
			}
		for(int j=0;j<D.lente[i].Sup.Q.size();j++){
			sfx=fx.lente[i].Sup.Q[j];
			sbx=bx.lente[i].Sup.Q[j];
			fx.lente[i].Sup.Q[j] += e2;
			bx.lente[i].Sup.Q[j] -= e2;
			R.lente[i].Sup.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2*(j+1));
			fx.lente[i].Sup.Q[j]=sfx;
			bx.lente[i].Sup.Q[j]=sbx;
			}
		}
	}

void GlobalUpdate(Sistema& D, Sistema& R, int i, int j, double eps){
	if (j<0) return;
	if (i<0) return;
	double sfx,sbx;
	Sistema fx = D;
	Sistema bx = D;
	R = Sistema(D);
		if(j<D.lente[i].Inf.Q.size()){
			sfx=fx.lente[i].Inf.Q[j];
			sbx=bx.lente[i].Inf.Q[j];
			fx.lente[i].Inf.Q[j] += e2;
			bx.lente[i].Inf.Q[j] -= e2;
			R.lente[i].Inf.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2);
			fx.lente[i].Inf.Q[j] = sfx;
			bx.lente[i].Inf.Q[j] = sbx;
			}

		if(j<D.lente[i].Sup.Q.size()){
			sfx=fx.lente[i].Sup.Q[j];
			sbx=bx.lente[i].Sup.Q[j];
			fx.lente[i].Sup.Q[j] += e2;
			bx.lente[i].Sup.Q[j] -= e2;
			R.lente[i].Sup.Q[j]  += (fx.GScore()-bx.GScore())*eps/(2*e2);
			fx.lente[i].Sup.Q[j]=sfx;
			bx.lente[i].Sup.Q[j]=sbx;
			}
	}
	
Lente& Lente::operator=(Lente&& bob){
    Lente* dest;
    std::memcpy(dest, bob, sizeof(Lente));
    return dest;
}	
    
*/
