#ifndef __f1_h_

#include "DNAFeature.h"

class f1 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif