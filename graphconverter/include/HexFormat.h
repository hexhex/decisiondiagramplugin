#ifndef _HEXFORMAT_H_
#define _HEXFORMAT_H_

#include <IFormat.h>

using namespace dlvhex::dd::util;

namespace dlvhex{
	namespace dd{
		namespace tools{
			namespace graphconverter{

				class HexFormat : public IFormat{
				public:
					virtual std::string getName();
					virtual std::string getNameAbbr();
					virtual std::vector<DecisionDiagram*> read() throw (DecisionDiagram::InvalidDecisionDiagram);
					virtual void write(std::vector<DecisionDiagram*> dd) throw (DecisionDiagram::InvalidDecisionDiagram);
				};
			}
		}
	}
}

#endif
