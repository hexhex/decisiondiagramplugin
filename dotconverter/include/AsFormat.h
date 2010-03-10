#ifndef _ASFORMAT_H_
#define _ASFORMAT_H_

#include <IFormat.h>

class AsFormat : public IFormat{
public:
	virtual std::string getName();
	virtual std::string getNameAbbr();
	virtual DecisionDiagram* read();
	virtual bool write(DecisionDiagram* dd);
};

#endif
