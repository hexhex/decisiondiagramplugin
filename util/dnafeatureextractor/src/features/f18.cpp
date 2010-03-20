#include "features/f18.h"

#include <math.h>
#include <stdlib.h>
#include "features/f9.h"
#include "features/f12.h"
#include "features/f15.h"

using namespace dlvhex::dd::tools::dna;

float f18::extract(string& sequence){
	f9 _f9;
	f12 _f12;
	f15 _f15;

	float y1 = abs(_f9.extract(sequence));
	float y2 = abs(_f12.extract(sequence));
	float y3 = abs(_f15.extract(sequence));
	return max(y1, max(y2, y3));
}
