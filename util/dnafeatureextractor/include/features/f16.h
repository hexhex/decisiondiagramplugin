#ifndef __f16_h_
#define __f16_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f16 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
