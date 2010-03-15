#ifndef __f8_h_
#define __f8_h_

#include "DNAFeature.h"

class f8 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif