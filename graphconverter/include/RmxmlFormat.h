#ifndef _RMXMLFORMAT_H_
#define _RMXMLFORMAT_H_

#include <IFormat.h>

#include <tinyxml.h>

class RmxmlFormat : public IFormat{
private:
	// XML --> DD
	DecisionDiagram getDDDiag(TiXmlDocument* doc);
	DecisionDiagram::Node* getDDnode(DecisionDiagram* dd, TiXmlElement* ce);
	DecisionDiagram::Edge* getDDedge(DecisionDiagram* dd, DecisionDiagram::Node* parent, DecisionDiagram::Node* child, TiXmlElement* con);

	// DD --> XML
	TiXmlDocument getXmlDiag(DecisionDiagram* dd);
	void getXmlDiag(DecisionDiagram* dd, DecisionDiagram::Node* node, TiXmlElement* parent, int& id);
	std::string getXmlCmpAttribute(DecisionDiagram::Condition c);
	std::string getXmlCmpValue(DecisionDiagram::Condition c);
	std::string getXmlCmpOperation(DecisionDiagram::Condition c);
public:
	virtual std::string getName();
	virtual std::string getNameAbbr();
	virtual DecisionDiagram* read();
	virtual bool write(DecisionDiagram* dd);
};

#endif