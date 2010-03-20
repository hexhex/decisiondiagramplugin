#include "features/f20.h"

using namespace dlvhex::dd::tools::dna;

float f20::extract(string& sequence){
	float Pin, Pout1, Pout2;
	float abs = 0.0f;
	float n = 0.0f;

	// Pin
	for (unsigned int k = 2; k < sequence.length(); k+=3){
		for (int s = 0; s < 3; s++){
			for (unsigned int b = 0; b + k < sequence.length(); b++){
				n++;
				if (sequence[b] == sequence[b + k]){
					abs++;
				}
			}
		}
	}
	Pin = (n > 0 ? abs / n : 0.0f);

	// Pout1
	abs = 0.0f;
	n = 0.0f;
	for (unsigned int k = 0; k < sequence.length(); k+=3){
		for (int s = 0; s < 3; s++){
			for (unsigned int b = s; b + k < sequence.length(); b++){
				n++;
				if (sequence[b] == sequence[b + k]){
					abs++;
				}
			}
		}
	}
	Pout1 = (n > 0 ? abs / n : 0.0f);

	// Pout1
	abs = 0.0f;
	n = 0.0f;
	for (unsigned int k = 1; k < sequence.length(); k+=3){
		for (int s = 0; s < 3; s++){
			for (unsigned int b = s; b + k < sequence.length(); b++){
				n++;
				if (sequence[b] == sequence[b + k]){
					abs++;
				}
			}
		}
	}
	Pout2 = (n > 0 ? abs / n : 0.0f);

	float c = max(Pin, max(Pout1, Pout2));
	float d = min(Pin, min(Pout1, Pout2));
	return (d > 0 ? c / d : 0.0f);
}
