#ifndef __f22_h_
#define __f22_h_

#include "DNAFeature.h"

class f22 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif