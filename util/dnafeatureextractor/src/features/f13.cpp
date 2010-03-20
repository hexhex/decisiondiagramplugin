#include "features/f13.h"

using namespace dlvhex::dd::tools::dna;

float f13::extract(string& sequence){
	float fbr[4][3];
	fbr[0][0] = fbr[0][1] = fbr[0][2] = 0.0f;
	fbr[1][0] = fbr[1][1] = fbr[1][2] = 0.0f;
	fbr[2][0] = fbr[2][1] = fbr[2][2] = 0.0f;
	fbr[3][0] = fbr[3][1] = fbr[3][2] = 0.0f;
	float pos[] = {0.0f, 0.0f, 0.0f};

	// count nucleotides
	for (unsigned int i = 0; i < sequence.length(); i++){
		pos[i % 3]++;
		switch(sequence[i]){
			case 'a':
				fbr[0][i % 3]++;
				break;
			case 't':
				fbr[1][i % 3]++;
				break;
			case 'g':
				fbr[2][i % 3]++;
				break;
			case 'c':
				fbr[3][i % 3]++;
				break;
		}
	}

	// compute relative frequency on each triplet position
	for (int base = 0; base < 4; base++){
		for (int tp = 0; tp < 3; tp++){
			fbr[base][tp] /= pos[tp];
		}
	}

	// x = (a + g) - (c + t)
	// y = (a + c) - (g + t)
	// z = (a + t) - (g + c)
	// pos=0
	return (fbr[0][1] + fbr[1][1]) - (fbr[2][1] + fbr[3][1]);	// z
}
