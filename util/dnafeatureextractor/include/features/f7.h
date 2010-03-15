#ifndef __f7_h_
#define __f7_h_

#include "DNAFeature.h"

class f7 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif