#ifndef __DNAFeature_h_
#define __DNAFeature_h_

#include <string>
using namespace std;

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class DNAFeature{
public:
	virtual float extract(string& sequence) = 0;
};

}
}
}
}

#endif
