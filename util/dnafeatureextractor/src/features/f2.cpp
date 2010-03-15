#include "features/f2.h"

float f2::extract(string& sequence){
	string atg("atg");
	// count number of ATG occurrances
	float n = 0.0f;
	for (unsigned int i = 0; i < sequence.length() - 3; i++){
		if (sequence.substr(i, 3) == atg){
			n++;
		}
	}
	return n;
}