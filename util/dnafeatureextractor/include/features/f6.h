#ifndef __f6_h_
#define __f6_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f6 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
