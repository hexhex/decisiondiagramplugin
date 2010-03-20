#include "features/f4.h"

using namespace dlvhex::dd::tools::dna;

float f4::extract(string& sequence){
	string taa("taa");
	string tag("tag");
	string tga("tga");
	// count number of TAA, TAG or TGA occurrances
	float n = 0.0f;
	for (unsigned int i = 0; i < sequence.length() - 3; i++){
		if (sequence.substr(i, 3) == taa || sequence.substr(i, 3) == tag || sequence.substr(i, 3) == tga){
			n++;
		}
	}
	return n;
}
