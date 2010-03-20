#ifndef __f21_h_
#define __f21_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f21 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
