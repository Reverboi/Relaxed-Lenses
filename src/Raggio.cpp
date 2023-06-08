#include "Raggio.hpp"

void Raggio :: Log(std::ofstream& fpt) const {
	if(fpt.is_open()) fpt<<X<<" "<<Y<<"\n";
	}
