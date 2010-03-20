#ifndef __f17_h_
#define __f17_h_

#include "DNAFeature.h"

namespace dlvhex{
namespace dd{
namespace tools{
namespace dna{

class f17 : public DNAFeature{
public:
	virtual float extract(string& sequence);
};

}
}
}
}

#endif
