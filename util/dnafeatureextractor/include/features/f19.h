#ifndef __f19_h_
#define __f19_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f19 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
