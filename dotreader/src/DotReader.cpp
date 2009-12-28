#include <graphviz/gvc.h>

#include <iostream>
#include <string>
#include <sstream>

std::string trim(std::string rs);
std::string writeNode(graph_t *g, Agnode_t *v);
std::string writeEdge(graph_t *g, Agedge_t *e);
std::string getNodeName(graph_t *g, Agnode_t *v);
std::string getNodeClassification(graph_t *g, Agnode_t *v);

std::string trim(std::string rs){
	int begin = rs.length() - 1;
	int end = 0;
	int j = 0;
	for (std::string::iterator it = rs.begin(); it != rs.end(); it++, j++){
		if (j < begin && (*it) != ' ' && (*it) != '\t' && (*it) != '\r' && (*it) != '\n') begin = j;
		if (j > end && (*it) != ' ' && (*it) != '\t' && (*it) != '\r' && (*it) != '\n') end = j;
	}
	if (end >= begin){
		rs = rs.substr(begin, end - begin + 1);
	}else{
		rs = std::string("");
	}
	return rs;
}

std::string getNodeName(graph_t *g, Agnode_t *v){
	std::string nodeLabel = std::string(v->name);

	// Extract node label
	int classStart = nodeLabel.find_first_of("[") + 1;
	int classEnd = nodeLabel.find_first_of("]") - 1;
	bool err = false;
	if (classStart != std::string::npos && classEnd != std::string::npos){
		if (classEnd >= classStart){
			return nodeLabel.substr(0, classStart - 1);
		}else{
			return nodeLabel;
		}
	}else{
		return nodeLabel;
	}
}

std::string getNodeClassification(graph_t *g, Agnode_t *v){
	std::string nodeLabel = std::string(v->name);

	// Extract classification
	int classStart = nodeLabel.find_first_of("[") + 1;
	int classEnd = nodeLabel.find_first_of("]") - 1;
	bool err = false;
	if (classStart != std::string::npos && classEnd != std::string::npos){
		if (classEnd >= classStart){
			return nodeLabel.substr(classStart, classEnd - classStart + 1);
		}else{
			return std::string("");
		}
	}else{
		return std::string("");
	}
}

std::string writeNode(graph_t *g, Agnode_t *v){
	std::stringstream ss;

	bool hasInEdges = false;
	bool hasOutEdges = false;

	Agedge_t *e;

	// Determine type of node
	for (e = agfstout(g,v); e; e = agnxtout(g,e)){
		hasOutEdges = true;
	}
	for (e = agfstin(g,v); e; e = agnxtin(g,e)){
		hasInEdges = true;
	}

	// Generate code for this node
	std::string nodeLabel = getNodeName(g, v);
	if (hasInEdges && hasOutEdges){
		ss << "innernode(" << nodeLabel << ").";
	}else if (hasInEdges){
		ss << "leafnode(" << trim(nodeLabel) << ", " << trim(getNodeClassification(g, v)) << ").";
	}else{
		ss << "root(" << nodeLabel << ").";
		ss << "innernode(" << nodeLabel << ").";
	}

	return ss.str();
}

std::string writeEdge(graph_t *g, Agedge_t *e){
	std::string edgeLabel = std::string(agget(e, (char*)"label"));
	std::stringstream ss;

	// Determine type of edges
	if (edgeLabel.length() == 0){
		ss << "elseedge(" << e->head->name << ", " << e->tail->name << ").";
	}else{
		// Conditinoal edge: Extract operands and operator of condition
		int operatorPos = -1;
		int operatorLength = -1;
		if (operatorPos < 0 && edgeLabel.find_first_of("<=") != std::string::npos) { operatorPos = edgeLabel.find_first_of("<="); operatorLength = 2; }
		if (operatorPos < 0 && edgeLabel.find_first_of(">=") != std::string::npos) { operatorPos = edgeLabel.find_first_of(">="); operatorLength = 2; }
		if (operatorPos < 0 && edgeLabel.find_first_of("<") != std::string::npos) { operatorPos = edgeLabel.find_first_of("<"); operatorLength = 1; }
		if (operatorPos < 0 && edgeLabel.find_first_of(">") != std::string::npos) { operatorPos = edgeLabel.find_first_of(">"); operatorLength = 1; }
		if (operatorPos < 0 && edgeLabel.find_first_of("=") != std::string::npos) { operatorPos = edgeLabel.find_first_of("="); operatorLength = 1; }
		std::string firstOperand = edgeLabel.substr(0, operatorPos);
		std::string secondOperand = edgeLabel.substr(operatorPos + operatorLength);
		std::string operator_ = edgeLabel.substr(operatorPos, operatorLength);

		// Generate code for this conditional edge
		ss << "edge(" << getNodeName(g, e->head) << ", " << getNodeName(g, e->tail) << ", " << trim(firstOperand) << ", \"" << trim(operator_) << "\", " << trim(secondOperand) << ").";
	}
	return ss.str();
}

int main(int argc, char **argv){

	// Let graphviz lib parse the standard input
	GVC_t *gvc;
	graph_t *g;
	FILE *fp;
	gvc = gvContext();
	fp = stdin;//fopen("/home/redl/Desktop/hello.dot", "r");
	g = agread(stdin);

	if (!g){
		std::cout << "Error while graph reading";
		return 1;
	}else{

		Agnode_t *v;
		Agedge_t *e;

		// Iterate through all nodes or the graph
		for (v = agfstnode(g); v; v = agnxtnode(g,v)){

			// Write node
			std::cout << writeNode(g, v) << std::endl;

			// Write outgoing edges
			for (e = agfstout(g,v); e; e = agnxtout(g,e)){
				std::cout << writeEdge(g, e) << std::endl;
			}
		}

		return 0;
	}
}
