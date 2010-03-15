#ifndef __f10_h_
#define __f10_h_

#include "DNAFeature.h"

class f10 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif