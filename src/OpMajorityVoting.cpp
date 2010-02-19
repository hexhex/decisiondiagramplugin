#include <OpMajorityVoting.h>
#include <DecisionDiagram.h>

#include <iostream>
#include <sstream>
#include <set>

using namespace dlvhex::dd;

std::string OpMajorityVoting::getName(){
	return "majorityvoting";
}

DecisionDiagram::Node* OpMajorityVoting::makeInnerNode(DecisionDiagram* dd, DecisionDiagram::Node* n_old){

	// Find a unique temporary name for the old leaf node and rename it (this makes the old label free for the inner node)
	std::string leaflabel = n_old->getLabel();
	std::string tmpleaflabel = dd->getUniqueLabel(leaflabel);
	n_old->setLabel(tmpleaflabel);

	// Add the new inner node
	DecisionDiagram::Node* innernode = dd->addNode(leaflabel);

	// Extract all edges into n_old or out of n_old
	std::set<DecisionDiagram::Edge*> in = n_old->getInEdges();
	std::set<DecisionDiagram::Edge*> out = n_old->getOutEdges();	// Should be empty since leaf nodes have no outgoing edges

	// Copy all edges and redirect them to the new inner node
	for (std::set<DecisionDiagram::Edge*>::iterator it = in.begin(); it != in.end(); it++){
		DecisionDiagram::Edge* e = *it;
		if (dynamic_cast<DecisionDiagram::ElseEdge*>(e) != NULL){
			dd->addElseEdge(e->getFrom(), innernode);
		}else{
			dd->addEdge(e->getFrom(), innernode, e->getCondition());
		}
	}
	for (std::set<DecisionDiagram::Edge*>::iterator it = out.begin(); it != out.end(); it++){
		DecisionDiagram::Edge* e = *it;
		if (dynamic_cast<DecisionDiagram::ElseEdge*>(e) != NULL){
			dd->addElseEdge(innernode, e->getTo());
		}else{
			dd->addEdge(innernode, e->getTo(), e->getCondition());
		}
	}

	// Remove the old node from the decision diagram (force edge removal)
	dd->removeNode(n_old, true);

	return innernode;
}

HexAnswer OpMajorityVoting::apply(int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters){
	try{
		// Check arity
		if (arity != 2){
			std::stringstream msg;
			msg << "majorityvoting is a binary (2-ary) operator. " << arity << " answers were passed.";
			throw IOperator::OperatorException(msg.str());
		}
		if (arguments[0]->size() != 1 || arguments[1]->size() != 1){
			std::stringstream msg;
			msg << "majorityvoting expects each answer to contain exactly one answer set.";
			throw IOperator::OperatorException(msg.str());
		}

		// Construct input decision diagrams
		DecisionDiagram dd1((*arguments[0])[0]);
		DecisionDiagram dd2((*arguments[1])[0]);

		// Merge the decision diagrams
		// Extract all distingt leaf nodes of dd1
		std::set<DecisionDiagram::LeafNode*> dd1leafs = dd1.getLeafNodes();

		// For each distinct leaf node (according to it's classification), make a copy of dd2
		std::map<std::string, DecisionDiagram> dd2copies;
		for (std::set<DecisionDiagram::LeafNode*>::iterator it = dd1leafs.begin(); it != dd1leafs.end(); it++){
			// Check if this classification is new
			if (dd2copies.find((*it)->getClassification()) == dd2copies.end()){
				// Make a copy of dd2
				dd2copies[(*it)->getClassification()] = DecisionDiagram(dd2);
			}
		}

		// Add all dd2 copies to the main decision diagram
		for (std::map<std::string, DecisionDiagram>::iterator copyIt = dd2copies.begin(); copyIt != dd2copies.end(); copyIt++){

			// Rename nodes if necessary
			(*copyIt).second.useUniqueLabels(&dd1);

			// All leafs of dd2 which come to the same classification as the leaf node in dd1 will remain unchanged; others are changed to "unknown"
			std::set<DecisionDiagram::LeafNode*> dd2leafs = (*copyIt).second.getLeafNodes();
			for (std::set<DecisionDiagram::LeafNode*>::iterator dd2it = dd2leafs.begin(); dd2it != dd2leafs.end(); dd2it++){
				if ((*dd2it)->getClassification() != (*copyIt).first){
					(*dd2it)->setClassification("unknown");
				}
			}

			// Add
			DecisionDiagram::Node* dd2root = dd1.addDecisionDiagram(&(*copyIt).second);

			// Connect all leafs of dd1 with the according classification to the root element of dd2
			for (std::set<DecisionDiagram::LeafNode*>::iterator dd1it = dd1leafs.begin(); dd1it != dd1leafs.end(); dd1it++){
				if ((*dd1it)->getClassification() == (*copyIt).first){
					// Convert leaf node into an inner node
					DecisionDiagram::Node* innernode = makeInnerNode(&dd1, *dd1it);
					// Connect it with the root of dd2
					dd1.addElseEdge(innernode, dd2root);
				}else{
				}
			}
		}

		// Convert the final decision diagram into a hex answer
		HexAnswer answer;
		answer.push_back(dd1.toAnswerSet());
		return answer;

	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
