#ifndef __f4_h_
#define __f4_h_

#include "DNAFeature.h"

class f4 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif