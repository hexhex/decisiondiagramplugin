#ifndef _IFORMAT_H_
#define _IFORMAT_H_

#include <DecisionDiagram.h>

#include <string>

using namespace dlvhex::dd;

class IFormat{
public:
	virtual std::string getName() = 0;
	virtual std::string getNameAbbr();

	virtual DecisionDiagram* read() throw (DecisionDiagram::InvalidDecisionDiagram) = 0;
	virtual void write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram) = 0;
};

#endif
