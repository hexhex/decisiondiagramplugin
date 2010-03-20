#ifndef __f20_h_
#define __f20_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f20 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
