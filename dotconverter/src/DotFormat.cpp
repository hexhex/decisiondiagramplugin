#include <DotFormat.h>

#include <StringHelper.h>

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <sstream>

std::string DotFormat::getName(){
	return "dot";
}

DecisionDiagram* DotFormat::read(){
	// Let graphviz lib parse the standard input
	GVC_t *gvc;
	graph_t *g;
	FILE *fp;
	gvc = gvContext();
	fp = stdin;
	g = agread(stdin);

	if (!g){
		std::cerr << "Error while graph reading";
		return NULL;
	}else{
		DecisionDiagram* dd = new DecisionDiagram();
		Agnode_t *v;
		Agedge_t *e;

		try{
			// Iterate through all nodes of the graph
			for (v = agfstnode(g); v; v = agnxtnode(g,v)){
				// create node
				makeDDNode(dd, g, v);
			}

			for (v = agfstnode(g); v; v = agnxtnode(g,v)){
				// create outgoing edges
				for (e = agfstout(g,v); e; e = agnxtout(g,e)){
					makeDDEdge(dd, g, e);
				}
			}
		}catch(dlvhex::dd::DecisionDiagram::InvalidDecisionDiagram idd){
			std::cerr << idd.getMessage();
			delete dd;
			return NULL;
		}
		return dd;
	}
}

bool DotFormat::write(DecisionDiagram* dd){
	std::cout << dd->toDotFileString();
	return true;
}

/**
 * Extracts the text assigned to a certain node
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 * \return std::string The label of node v
 */
std::string DotFormat::getNodeText(graph_t *g, Agnode_t *v){
	std::string text = std::string(v->name);
	return text;
}

/**
 * Translates a node of a graphviz graph into an DecisionDiagram::Node
 * \param dd Pointer to the target decision diagram
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 */
void DotFormat::makeDDNode(dlvhex::dd::DecisionDiagram* dd, graph_t *g, Agnode_t *v){

	bool hasInEdges = false;
	bool hasOutEdges = false;
	static bool rootWritten = false;

	Agedge_t *e;

	// Determine type of node
	for (e = agfstout(g,v); e; e = agnxtout(g,e)){
		hasOutEdges = true;
	}
	for (e = agfstin(g,v); e; e = agnxtin(g,e)){
		hasInEdges = true;
	}

	// Generate node
	std::string nodeName = StringHelper::extractLabel(getNodeText(g, v));
	if (hasInEdges && hasOutEdges){
		dd->addNode(nodeName);
	}else if (hasInEdges){
		dd->addLeafNode(nodeName, StringHelper::extractClassification(getNodeText(g, v)));
	}else{		
		if (!rootWritten){
			dd->setRoot(dd->addNode(nodeName));
		}else{
			dd->addNode(nodeName);
		}
		rootWritten = true;	// Write root only once; if the graph is a forest (rather than a tree), the first root discovered is used
	}

}


/**
 * Translates an edge of a graphviz graph into a DecisionDiagram::Edge
 * \param dd Pointer to the target decision diagram
 * \param g Pointer to a graphviz graph
 * \param e Pointer to an edge of graph g
 */
void DotFormat::makeDDEdge(dlvhex::dd::DecisionDiagram* dd, graph_t *g, Agedge_t *e){
	std::string edgeLabel = std::string(agget(e, (char*)"label"));
	std::stringstream ss;

	// Determine type of edges
	if (edgeLabel.length() == 0 || edgeLabel == std::string("else")){
		dd->addElseEdge(dd->getNodeByLabel(e->tail->name), dd->getNodeByLabel(e->head->name));
	}else{
		// Conditinoal edge: Extract operands and operator of condition
		int operatorPos = -1;
		int operatorLength = -1;
		if (operatorPos < 0 && edgeLabel.find("<=") != std::string::npos) { operatorPos = edgeLabel.find_first_of("<="); operatorLength = 2; }
		if (operatorPos < 0 && edgeLabel.find(">=") != std::string::npos) { operatorPos = edgeLabel.find_first_of(">="); operatorLength = 2; }
		if (operatorPos < 0 && edgeLabel.find("<") != std::string::npos) { operatorPos = edgeLabel.find_first_of("<"); operatorLength = 1; }
		if (operatorPos < 0 && edgeLabel.find(">") != std::string::npos) { operatorPos = edgeLabel.find_first_of(">"); operatorLength = 1; }
		if (operatorPos < 0 && edgeLabel.find("=") != std::string::npos) { operatorPos = edgeLabel.find_first_of("="); operatorLength = 1; }
		std::string firstOperand = edgeLabel.substr(0, operatorPos);
		std::string secondOperand = edgeLabel.substr(operatorPos + operatorLength);
		std::string operator_ = edgeLabel.substr(operatorPos, operatorLength);
		boost::trim(firstOperand);
		boost::trim(secondOperand);

		// Generate code for this conditional edge
		dd->addEdge(dd->getNodeByLabel(e->tail->name), dd->getNodeByLabel(e->head->name), dlvhex::dd::DecisionDiagram::Condition(firstOperand, secondOperand, operator_));
	}
}
