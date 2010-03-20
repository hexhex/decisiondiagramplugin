#ifndef __f1_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f1 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
