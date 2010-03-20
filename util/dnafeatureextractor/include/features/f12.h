#ifndef __f12_h_
#define __f12_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f12 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
