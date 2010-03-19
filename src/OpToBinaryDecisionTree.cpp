#include <OpToBinaryDecisionTree.h>
#include <DecisionDiagram.h>

#include <sstream>
#include <set>

using namespace dlvhex::dd;

std::string OpToBinaryDecisionTree::getName(){
	return "tobinarydecisiontree";
}

void OpToBinaryDecisionTree::toBinary(DecisionDiagram& dd, DecisionDiagram::Node* root){

	// Check arity of root node
	if (root->getOutEdgesCount() > 2){
		// greater than 2 --> needs to be transformed

		bool firstOutEdge = true;
		
		// Create a new intermediate node (with a label similar to the original name)
		DecisionDiagram::Node* intermediateNode = dd.addNode(dd.getUniqueLabel(root->getLabel()));

		std::set<DecisionDiagram::Edge*> oedges = root->getOutEdges();
		DecisionDiagram::Edge* selectedEdge = NULL;
		for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
			// Just take the first conditional edge as it is
			// Note: In theory, the order in which the conditions are checked is irrelevant since the resulting diagrams are equivalent in any case.
			//       However, it makes writing test cases much easier if the behaviour is deterministic. Therefore we look for the
			//       lexically smallest condition and check it first.
			if (dynamic_cast<DecisionDiagram::ElseEdge*>(*it) == NULL &&
								(selectedEdge == NULL || (*it)->getCondition().toString().compare(selectedEdge->getCondition().toString()) < 0)){
				selectedEdge = *it;
			}
		}

		// unfold all child nodes
		for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
			if (*it == selectedEdge){
				firstOutEdge = false;
				// Convert the sub decision diagram into a binary one
				toBinary(dd, (*it)->getTo());
			}else{
				// All other edges are redirected such that the start at the intermediate node
				DecisionDiagram::Node *currentSubNode = (*it)->getTo();
				dd.addEdge(intermediateNode, currentSubNode, (*it)->getCondition());

				// Remove the original edge
				dd.removeEdge(*it);

				// Convert the sub decision diagram into a binary one
				toBinary(dd, (*it)->getTo());
			}
		}

		// Let the intermediate node be the root's else child
		DecisionDiagram::Edge *e = dd.addElseEdge(root, intermediateNode);
	}else if(root->getOutEdgesCount() == 2){
		// leave it as it is --> just transform the child nodes
		std::set<DecisionDiagram::Edge*> oedges = root->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
			toBinary(dd, (*it)->getTo());
		}
	}else{
		// either a leaf node --> leave it as it is
		// or: an inner node with an illegal arity
		if (!dynamic_cast<DecisionDiagram::LeafNode*>(root)){
			throw IOperator::OperatorException(std::string("Discovered an inner node with an illegal arity (must be >= 2): ") + root->getLabel());
		}
	}
}

HexAnswer OpToBinaryDecisionTree::apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException){

	try{
		// Check arity
		if (arity != 1){
			std::stringstream msg;
			msg << "tobinarydecisiontree is a unary operator. " << arity << " answers were passed.";
			throw IOperator::OperatorException(msg.str());
		}

		// execute operator for each input decision diagram
		HexAnswer answer;
		for (int answerSetNr = 0; answerSetNr < answers[0]->size(); answerSetNr++){
			// Construct input decision diagram
			DecisionDiagram dd((*answers[0])[answerSetNr]);

			// Check preconditions
			if (!dd.isTree()){
				throw IOperator::OperatorException("tobinarydecisiontree expects a decision tree, but the given decision diagram is not a tree.");
			}

			// Convert it into a binary one
			toBinary(dd, dd.getRoot());

			// Convert the final decision diagram into a hex answer
			answer.push_back(dd.toAnswerSet());
		}

		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram ide){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + ide.getMessage());
	}
}

