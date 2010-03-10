#ifndef _IFORMAT_H_
#define _IFORMAT_H_

#include <DecisionDiagram.h>

#include <string>

using namespace dlvhex::dd;

class IFormat{
public:
	virtual std::string getName() = 0;
	virtual std::string getNameAbbr();

	virtual DecisionDiagram* read() = 0;
	virtual bool write(DecisionDiagram* dd) = 0;
};

#endif
