#ifndef __f13_h_
#define __f13_h_

#include "DNAFeature.h"

class f13 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif