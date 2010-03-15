#ifndef __f14_h_
#define __f14_h_

#include "DNAFeature.h"

class f14 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif