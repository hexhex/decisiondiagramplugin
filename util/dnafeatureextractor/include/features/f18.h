#ifndef __f18_h_
#define __f18_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f18 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
