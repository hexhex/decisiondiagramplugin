#ifndef _HEXFORMAT_H_
#define _HEXFORMAT_H_

#include <IFormat.h>

class HexFormat : public IFormat{
public:
	virtual std::string getName();
	virtual std::string getNameAbbr();
	virtual DecisionDiagram* read();
	virtual bool write(DecisionDiagram* dd);
};

#endif
