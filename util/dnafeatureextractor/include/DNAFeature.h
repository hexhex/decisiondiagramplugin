#ifndef __DNAFeature_h_
#define __DNAFeature_h_

#include <string>
using namespace std;

class DNAFeature{
public:
	virtual float extract(string& sequence) = 0;
};

#endif