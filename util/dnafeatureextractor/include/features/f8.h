#ifndef __f8_h_
#define __f8_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f8 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
