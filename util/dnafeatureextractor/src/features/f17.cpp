#include "features/f17.h"

#include <math.h>
#include <stdlib.h>
#include "features/f8.h"
#include "features/f11.h"
#include "features/f14.h"

using namespace dlvhex::dd::tools::dna;

float f17::extract(string& sequence){
	f8 _f8;
	f11 _f11;
	f14 _f14;

	float x1 = abs(_f8.extract(sequence));
	float x2 = abs(_f11.extract(sequence));
	float x3 = abs(_f14.extract(sequence));
	return max(x1, max(x2, x3));
}
