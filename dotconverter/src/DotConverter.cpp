#include <graphviz/gvc.h>

#include <iostream>
#include <string>
#include <sstream>

std::string trim(std::string rs);
std::string writeNode(graph_t *g, Agnode_t *v);
std::string writeEdge(graph_t *g, Agedge_t *e);
std::string getNodeName(graph_t *g, Agnode_t *v);
std::string getNodeClassification(graph_t *g, Agnode_t *v);

// ==================== String helper functions ====================

/**
 * Cuts off leading and successional whitespaces
 * \param rs An arbitrary string
 * \return std::string rs with leading and successional blanks, tabs and newlines cut off
 */
std::string trim(std::string rs){
	if (rs.length() == 0) return rs;

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

/**
 * Removes leading and successional quotes from strings
 * \param rs An arbitrary string
 * \return std::string rs with leading and successional quotes (") cut off
 */
std::string unquote(std::string rs){
	if (rs.length() >= 2 && rs[0] == '\"' && rs[rs.length() - 1] == '\"') return rs.substr(1, rs.length() - 2);
	else return rs;
}


// ==================== Functions for conversion of a graphviz graph into an answer set ====================

/**
 * Extracts the node name from a string containing either 1. the node name alone or 2. the node name together with a classification (as used in leaf nodes).
 * Example: "my_node", "my_leaf_node[my_classification]"
 * The method would return "my_node" resp. "my_leaf_node"
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 * \return std::string The label of node v
 */
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

/**
 * Extracts the classification from a string containing either 1. the node name alone or 2. the node name together with a classification (as used in leaf nodes).
 * In case that a string does not contain a classification, an empty string will be returned.
 * Example: "my_node", "my_leaf_node[my_classification]"
 * The method would return "" resp. "my_classification"
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 * \return std::string The classification of node v if present, otherwise the empty string
 */
std::string getNodeClassification(graph_t *g, Agnode_t *v){
	std::string nodeLabel = agget(v, (char*)"label");

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

/**
 * Translates a node of a graphviz graph into an answer set representation.
 * \param g Pointer to a graphviz graph
 * \param v Pointer to a node of graph g
 * \return std::string An atom (as string) representing the node.
 */
std::string writeNodeAsAtom(graph_t *g, Agnode_t *v){
	std::stringstream ss;

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

	// Generate code for this node
	std::string nodeName = getNodeName(g, v);
	if (hasInEdges && hasOutEdges){
		ss << "innernode(" << nodeName << ").";
	}else if (hasInEdges){
		ss << "leafnode(" << trim(nodeName) << ", " << trim(getNodeClassification(g, v)) << ").";
	}else{
		if (!rootWritten) ss << "root(" << nodeName << ")." << std::endl;
		ss << "innernode(" << nodeName << ").";
		rootWritten = true;	// Write root only once; if the graph is a forest (rather than a tree), the first root discovered is used
	}

	return ss.str();
}

/**
 * Translates an edge of a graphviz graph into an answer set representation.
 * \param g Pointer to a graphviz graph
 * \param e Pointer to an edge of graph g
 * \return std::string An atom (as string) representing the edge.
 */
std::string writeEdgeAsAtom(graph_t *g, Agedge_t *e){
	std::string edgeLabel = std::string(agget(e, (char*)"label"));
	std::stringstream ss;

	// Determine type of edges
	if (edgeLabel.length() == 0 || edgeLabel == std::string("else")){
		ss << "elseedge(" << e->tail->name << ", " << e->head->name << ").";
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

		// Generate code for this conditional edge
		ss << "conditionaledge(" << getNodeName(g, e->tail) << ", " << getNodeName(g, e->head) << ", " << trim(firstOperand) << ", \"" << trim(operator_) << "\", " << trim(secondOperand) << ").";
	}

	return ss.str();
}


// ==================== Functions for conversion of an answer set into a dot file ====================

/**
 * Strips off all the delimiter characters of a textual answer set representation and returns the chunks (predicate names, ground terms, literals) one after the other.
 * \param input Reference to the textual input.
 * \param cursor Reference to an integer with the current cursor position. The next chunk after this position will be returned and the cursor will be adjusted accordingly.
 * \param chunk A reference to a string which will store the next chunk beginning at the cursor position (or later) after the function returns.
 * \return bool True if a chunk was found, otherwise (end of file) false
 */
bool readChunk(std::string& input, int& cursor, std::string& chunk){
	chunk.clear();

	// Read sequence of alphanumerics or string literal inclosed by double quotes
	bool quoted = false;
	bool chunkStarted = false;
	int chunkStart;
	for (; cursor < input.length(); cursor++){
		char c = input[cursor];
		if (!quoted){
			if (	(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9') ||
				(c == '_') ){
				// Take character
				if (!chunkStarted) chunkStart = cursor;
				chunkStarted = true;
			}else if (c == '\"'){
				quoted = true;
				if (!chunkStarted) chunkStart = cursor;
				chunkStarted = true;
			}else{
				// All other characters terminate the chunk
				if (chunkStarted){
					chunk.append(input.substr(chunkStart, cursor - chunkStart));
					return true;
				}else{
					// Chunk start was not found yet: just skip unreognized characters
				}
			}
		}else{
			// Take everythink different from "
			if (c == '\"') quoted = false;
		}
	}
	// No more chunks
	return false;
}


// ==================== Main program ====================
// Recognized parameters:
//   --help		... Output description of the program and it's parameters
//   --toas		... Convert a dot file (read from standard input)
//			    into an answer set representation (written to
//			    standard output)
//			    This is the default mode if no option is proveded!
//   --todot		... Convert an answer set representation (read from
//			    standard input) of a graph into a dot file (written
//			    to standard output)
int main(int argc, char **argv){

	bool mode = true;	// true --> toas, false --> todot

	// Process parameters
	if (argc > 1){
		if (argc > 2){
			std::cerr << "Only 1 command line option is allowed, " << (argc - 1) << " were passed" << std::endl;
			return 1;
		}else{
			std::string param(argv[1]);
			if (param == std::string("--help")){
				std::cout	<<	"dotreader" << std::endl
						<<	"---------" << std::endl
						<<	"Recognized parameters:" << std::endl
						<<	" --help      ... Output description of the program and it's parameters" << std::endl
						<<	" --toas      ... Convert a dot file (read from standard input)" << std::endl
						<<	"                 into an answer set representation (written to" << std::endl
						<<	"                 standard output)" << std::endl
						<<	"                 This is the default mode if no option is proveded!" << std::endl
						<<	" --todot     ... Convert an answer set representation (read from" << std::endl
						<<	"                 standard input) of a graph into a dot file (written" << std::endl
						<<	"                 to standard output)" << std::endl << std::endl;
				return 0;
			}
			else if (param == std::string("--toas")) mode = true;
			else if (param == std::string("--todot")) mode = false;
			else{
				std::cerr << "Unrecognized option: \"" << param << "\"" << std::endl;
				return 1;
			}
		}
	}


	if (mode){
		// toas
		// A dot file is converted into an answer set representation

		// Let graphviz lib parse the standard input
		GVC_t *gvc;
		graph_t *g;
		FILE *fp;
		gvc = gvContext();
		fp = stdin;
		g = agread(stdin);

		if (!g){
			std::cout << "Error while graph reading";
			return 1;
		}else{

			Agnode_t *v;
			Agedge_t *e;

			// Iterate through all nodes of the graph
			for (v = agfstnode(g); v; v = agnxtnode(g,v)){

				// Write node
				std::cout << writeNodeAsAtom(g, v) << std::endl;

				// Write outgoing edges
				for (e = agfstout(g,v); e; e = agnxtout(g,e)){
					std::cout << writeEdgeAsAtom(g, e) << std::endl;
				}
			}

			return 0;
		}
	}else{
		// todot
		// An answer set representation is converted into a dot file

		// Read standard input
		std::stringstream inputss;
		std::string line;
		std::string input;
		while(getline(std::cin, line)) {
			inputss << line << std::endl;
		}
		input = inputss.str();

		// Read all chunks
		int cursor = 0;
		std::string cmdChunk;
		std::string trash;
		std::cout << "digraph {" << std::endl;
		while (readChunk(input, cursor, cmdChunk)){

			// Create list of edges (nodes are in general generated implicitly, but we write it explicitly since some nodes could be disconnected)
			if (cmdChunk == std::string("conditionaledge")){
				// Conditional edge
				std::string from;
				std::string to;
				std::string operand1;
				std::string operator_;
				std::string operand2;
				readChunk(input, cursor, from);
				readChunk(input, cursor, to);
				readChunk(input, cursor, operand1);
				readChunk(input, cursor, operator_);
				readChunk(input, cursor, operand2);
				std::cout << "     " << from << " -> " << to << " [label=\"" << operand1 << unquote(operator_) << operand2 << "\"]" << ";" << std::endl;
			}else if (cmdChunk == std::string("elseedge")){
				// Else edge
				std::string from;
				std::string to;
				readChunk(input, cursor, from);
				readChunk(input, cursor, to);
				std::cout << "     " << from << " -> " << to << " [label=\"else\"];" << std::endl;
			}else if (cmdChunk == std::string("innernode")){
				std::string name;
				std::string label;
				readChunk(input, cursor, name);
				if (name.find_first_of('_') != std::string::npos){
					label = name.substr(0, name.find_first_of('_'));
				}else{
					label = name;
				}
				std::cout << "     " << name << " [label=\"" << label << "\"];" << std::endl;
			}else if (cmdChunk == std::string("leafnode")){
				std::string name;
				std::string label;
				std::string classification;
				readChunk(input, cursor, name);
				readChunk(input, cursor, classification);
				if (name.find_first_of('_') != std::string::npos){
					label = name.substr(0, name.find_first_of('_'));
				}else{
					label = name;
				}
				std::cout << "     " << name << " [label=\"" << label << " [" << classification << "]\"];" << std::endl;
			}else if (cmdChunk == std::string("root")){
				readChunk(input, cursor, trash);
			}else{
				std::cerr << "Unrecognized token: \"" << cmdChunk << "\"" << std::endl;
				return 1;
			}

		}
		std::cout << "}" << std::endl;

		return 0;
	}
}
