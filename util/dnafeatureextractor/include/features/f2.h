#ifndef __f2_h_
#define __f2_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f2 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
