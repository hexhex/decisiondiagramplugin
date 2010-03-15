#ifndef __f9_h_
#define __f9_h_

#include "DNAFeature.h"

class f9 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif