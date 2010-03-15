#ifndef __f3_h_
#define __f3_h_

#include "DNAFeature.h"

class f3 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif