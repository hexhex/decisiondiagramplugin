#ifndef __f11_h_
#define __f11_h_

#include "DNAFeature.h"

class f11 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif