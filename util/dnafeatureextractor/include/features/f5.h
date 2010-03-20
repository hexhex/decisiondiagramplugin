#ifndef __f5_h_
#define __f5_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f5 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
