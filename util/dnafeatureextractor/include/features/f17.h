#ifndef __f17_h_
#define __f17_h_

#include "DNAFeature.h"

class f17 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

#endif