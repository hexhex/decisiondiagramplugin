#ifndef __f15_h_
#define __f15_h_

#include "DNAFeature.h"

class f15 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif