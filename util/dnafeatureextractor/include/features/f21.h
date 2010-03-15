#ifndef __f21_h_
#define __f21_h_

#include "DNAFeature.h"

class f21 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif