#include <OpUnfold.h>
#include <DecisionDiagram.h>

#include <sstream>
#include <set>

using namespace dlvhex::dd::util;
using namespace dlvhex::dd::plugin;

std::string OpUnfold::getName(){
	return "unfold";
}

DecisionDiagram OpUnfold::unfold(DecisionDiagram::Node* root, DecisionDiagram& ddin){

	DecisionDiagram ddResult;
	DecisionDiagram::Node* newRoot;
	if (dynamic_cast<DecisionDiagram::LeafNode*>(root) != NULL){
		// Just copy the leaf node and use it in a new decision diagram
		newRoot = ddResult.addNode(root);
		ddResult.setRoot(newRoot);
	}else{
		// Copy the current root node
		newRoot = ddResult.addNode(root);
		ddResult.setRoot(newRoot);

		// Unfold all child decision diagrams
		std::set<DecisionDiagram::Edge*> outEdges = root->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator childIt = outEdges.begin(); childIt != outEdges.end(); childIt++){
			DecisionDiagram::Node *currentChild = (*childIt)->getTo();
			DecisionDiagram unfoldedChild = unfold(currentChild, ddin);

			// Avoid duplicate node names
			unfoldedChild.useUniqueLabels(&ddResult);
			DecisionDiagram::Node *childRoot = ddResult.addDecisionDiagram(&unfoldedChild);

			// Connect the current root node with this child
			ddResult.addEdge(newRoot, childRoot, (*childIt)->getCondition());
		}
	}

	return ddResult;
}

HexAnswer OpUnfold::apply(int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters) throw (OperatorException){
	try{
		// Unfold all answer-sets
		HexAnswer result;
		for (int answerSetNr = 0; answerSetNr < arguments[0]->size(); answerSetNr++){
			// Construct a decision diagram from the answer set
			AtomSet as = (*arguments[0])[answerSetNr];
			DecisionDiagram dd(as);

			// Check for cycles in the input decision diagram
			std::vector<DecisionDiagram::Node*> cycle;
			if (!(cycle = dd.containsCycles()).empty()){
				std::stringstream cyclestring;
				bool first = true;
				for (std::vector<DecisionDiagram::Node*>::iterator it = cycle.begin(); it != cycle.end(); it++){
					cyclestring << (first ? "" : ", ") << (*it)->getLabel();
					first = false;
				}

				throw DecisionDiagram::InvalidDecisionDiagram("Cycle detected: " + cyclestring.str());
			}

			// Unfold the decision diagram
			dd = unfold(dd.getRoot(), dd);
			// The unfolded decision diagram does not need to be checked for cycles.
			// Since unfolding is equivalence preserving, there can be no cycles if there were none in the input decision diagram.

			// Translate the decision diagram back into an answer set
			result.push_back(dd.toAnswerSet());
		}
		return result;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
