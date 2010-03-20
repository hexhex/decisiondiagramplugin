#include "features/f1.h"

using namespace dlvhex::dd::tools::dna;

float f1::extract(string& sequence){
	string atg("atg");
	// count number of ATG occurrances
	float n = 0.0f;
	int k[] = {0, 0, 0};
	for (unsigned int i = 0; i < sequence.length() - 3; i++){
		if (sequence.substr(i, 3) == atg){
			n++;
			k[i % 3]++;
		}
	}
	float K = 0.0f;
	if (k[0] > 0) K = 1;
	if (k[1] > 0) K = 2;
	if (k[2] > 0) K = 3;
	return (1 + K * K) * n;
}
