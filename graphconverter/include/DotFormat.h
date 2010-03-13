#ifndef _DOTFORMAT_H_
#define _DOTFORMAT_H_

#include <IFormat.h>

#include <string>

#include <graphviz/gvc.h>

class DotFormat : public IFormat{
private:
	std::string getNodeText(graph_t *g, Agnode_t *v);
	std::string getNodeLabel(graph_t *g, Agnode_t *v);
	void makeDDNode(dlvhex::dd::DecisionDiagram* dd, graph_t *g, Agnode_t *v);
	void makeDDEdge(dlvhex::dd::DecisionDiagram* dd, graph_t *g, Agedge_t *e);
public:
	virtual std::string getName();

	virtual DecisionDiagram* read() throw (DecisionDiagram::InvalidDecisionDiagram);
	virtual void write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram);
};

#endif
