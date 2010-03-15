#include "features/f3.h"

float f3::extract(string& sequence){
	string taa("taa");
	string tag("tag");
	string tga("tga");
	// count number of TAA, TAG or TGA occurrances
	float n = 0.0f;
	int k[] = {0, 0, 0};
	for (unsigned int i = 0; i < sequence.length() - 3; i++){
		if (sequence.substr(i, 3) == taa || sequence.substr(i, 3) == tag || sequence.substr(i, 3) == tga){
			n++;
			k[i % 3]++;
		}
	}
	float K = 0.0f;
	if (k[0] > 0) K = 1.0f;
	if (k[1] > 0) K = 2.0f;
	if (k[2] > 0) K = 3.0f;
	return (1 + K * K) * n;
}