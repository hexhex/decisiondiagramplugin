#include <DotFormat.h>

#include <StringHelper.h>

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <sstream>

using namespace dlvhex::dd::tools::graphconverter;

std::string DotFormat::getName(){
	return "dot";
}

std::vector<DecisionDiagram*> DotFormat::read() throw (DecisionDiagram::InvalidDecisionDiagram){
	// Let graphviz lib parse the standard input
	GVC_t *gvc;
	graph_t *g;
	FILE *fp;
	gvc = gvContext();
	fp = stdin;
	g = agread(stdin);

	if (!g){
		throw DecisionDiagram::InvalidDecisionDiagram("Error: Error while graphviz was reading the diagram. It seems that the file does not contain a valid dot graph.");
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
		}catch(DecisionDiagram::InvalidDecisionDiagram idd){
			delete dd;
			throw idd;
		}
		std::vector<DecisionDiagram*> vec;
		vec.push_back(dd);
		return vec;
	}
}

void DotFormat::write(std::vector<DecisionDiagram*> ddv) throw (DecisionDiagram::InvalidDecisionDiagram){
	for (int i = 0; i < ddv.size(); i++){
		DecisionDiagram* dd = ddv[i];

		// retrieve components of decision diagram
		std::set<DecisionDiagram::Node*> nodes = dd->getNodes();
		std::set<DecisionDiagram::Edge*> edges = dd->getEdges();

		std::cout << "digraph {" << std::endl;

		// Create list of edges
		for (std::set<DecisionDiagram::Edge*>::iterator it = edges.begin(); it != edges.end(); it++){
			DecisionDiagram::Edge* e = *it;
			if (dynamic_cast<DecisionDiagram::ElseEdge*>(e) != NULL){
				std::cout << "     " << e->getFrom()->getLabel() << " -> " << e->getTo()->getLabel() << " [label=\"else\"];" << std::endl;
			}else{
				std::cout << "     " << e->getFrom()->getLabel() << " -> " << e->getTo()->getLabel() << " [label=\"" << e->getCondition().toString() << "\"]" << ";" << std::endl;
			}
		}

		// Create a list of leaf nodes
		for (std::set<DecisionDiagram::Node*>::iterator it = nodes.begin(); it != nodes.end(); it++){
			DecisionDiagram::Node* n = *it;

			// extract a reasonable label for this node
			std::string lab = n->getLabel();
			if (lab.find_first_of("_") != std::string::npos) lab = lab.substr(0, lab.find_first_of("_"));
			if (dynamic_cast<DecisionDiagram::LeafNode*>(n) != NULL){
				std::cout << "     " << n->getLabel() << " [label=\"" << lab << " [" << dynamic_cast<DecisionDiagram::LeafNode*>(n)->getClassification() << "]\"];" << std::endl;
			}else{
				std::cout << "     " << n->getLabel() << " [label=\"" << lab << "\"];" << std::endl;
			}
		}

		std::cout << "}" << std::endl;
	}
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
 * Extracts the label assigned to a certain node
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 * \return std::string The label of node v
 */
std::string DotFormat::getNodeLabel(graph_t *g, Agnode_t *v){
	std::string label = agget(v, (char*)"label");
	return label;
}

/**
 * Translates a node of a graphviz graph into an DecisionDiagram::Node
 * \param dd Pointer to the target decision diagram
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 */
void DotFormat::makeDDNode(DecisionDiagram* dd, graph_t *g, Agnode_t *v){

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
		dd->addLeafNode(nodeName, StringHelper::extractClassification(getNodeLabel(g, v)));
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
void DotFormat::makeDDEdge(DecisionDiagram* dd, graph_t *g, Agedge_t *e){
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

		// check if operator was found
		if (operatorLength != -1){
			// extract operands and operator
			std::string firstOperand = edgeLabel.substr(0, operatorPos);
			std::string secondOperand = edgeLabel.substr(operatorPos + operatorLength);
			std::string operator_ = edgeLabel.substr(operatorPos, operatorLength);
			boost::trim(firstOperand);
			boost::trim(secondOperand);

			// Generate code for this conditional edge
			dd->addEdge(dd->getNodeByLabel(e->tail->name), dd->getNodeByLabel(e->head->name), DecisionDiagram::Condition(firstOperand, secondOperand, operator_));
		}else{
			throw DecisionDiagram::InvalidDecisionDiagram(std::string("Comparison operator in expression \"") + edgeLabel + std::string("\" could not be detected"));
		}
	}
}
