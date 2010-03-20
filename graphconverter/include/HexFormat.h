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
					virtual DecisionDiagram* read() throw (DecisionDiagram::InvalidDecisionDiagram);
					virtual void write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram);
				};
			}
		}
	}
}

#endif
