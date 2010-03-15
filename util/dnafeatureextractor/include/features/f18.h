#ifndef __f18_h_
#define __f18_h_

#include "DNAFeature.h"

class f18 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif