#include "features/f19.h"

#include <math.h>
#include <stdlib.h>
#include "features/f10.h"
#include "features/f13.h"
#include "features/f16.h"

using namespace dlvhex::dd::tools::dna;

float f19::extract(string& sequence){
	f10 _f10;
	f13 _f13;
	f16 _f16;

	float z1 = abs(_f10.extract(sequence));
	float z2 = abs(_f13.extract(sequence));
	float z3 = abs(_f16.extract(sequence));
	return max(z1, max(z2, z3));
}
