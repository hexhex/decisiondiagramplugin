#ifndef _ASFORMAT_H_
#define _ASFORMAT_H_

#include <IFormat.h>
#include <DecisionDiagram.h>

class AsFormat : public IFormat{
public:
	virtual std::string getName();
	virtual std::string getNameAbbr();
	virtual DecisionDiagram* read() throw (DecisionDiagram::InvalidDecisionDiagram);
	virtual void write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram);
};

#endif
