#include "Raggio.hpp"
using namespace RelaxedLenses;
void Raggio :: Log(std::ofstream& fpt) const {
	if(fpt.is_open()) fpt<<X<<" "<<Y<<"\n";
	}
