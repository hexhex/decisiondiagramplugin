#include <OpOrderBinaryDecisionTree.h>
#include <DecisionDiagram.h>

#include <sstream>
#include <set>

using namespace dlvhex::dd::util;
using namespace dlvhex::dd::plugin;

std::string OpOrderBinaryDecisionTree::getName(){
	return "orderbinarydecisiontree";
}

std::string OpOrderBinaryDecisionTree::getInfo(){
	std::stringstream ss;
	ss <<	"   orderbinarydecisiontree" << std::endl <<
		"   -----------------------"  << std::endl << std::endl <<
		 "This class implements the tree ordering operator. It assumes each answer to represent a binary decision tree and translates it into an ordered one." << std::endl <<
		 "Usage:" << std::endl <<
		 "&operator[\"orderbinarydecisiontree\", DD, K](A)" << std::endl <<
		 "   DD     ... predicate with index 0 and handle to exactly 1 answer containing arbitrary many decision diagram" << std::endl <<
		 "   A      ... answer to the operator result (answer containing ordered binary decision diagrams)";
	return ss.str();

}

std::string OpOrderBinaryDecisionTree::getCompareAttribute(DecisionDiagram::Node* node){
	// Search for the requested attribute in the outgoing edges of this node
	std::string attr;
	bool attrFound = false;
	std::set<DecisionDiagram::Edge*> oedges = node->getOutEdges();
	for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
		DecisionDiagram::Condition condition = (*it)->getCondition();
		if (condition.getOperation() != DecisionDiagram::Condition::else_){
			if (attrFound){
				std::stringstream msg;
				msg << "Node \"" << node->getLabel() << "\" has more than one outgoing conditional edge";
				throw IOperator::OperatorException(msg.str());
			}
			attr = condition.getOperand1();
			attrFound = true;
		}
	}
	if (!attrFound){
		std::stringstream msg;
		msg << "Node \"" << node->getLabel() << "\" has no outgoing conditional edge";
		throw IOperator::OperatorException(msg.str());
	}
	return attr;
}

// sinks a given node until it is smaller than each of it's children (similat to Heap sort)
DecisionDiagram::Node* OpOrderBinaryDecisionTree::sink(DecisionDiagram& dd, DecisionDiagram::Node* root){
	// We only work with binary decision trees
	if (root->getOutEdgesCount() != 2){
		std::stringstream msg;
		msg << "Error: Decision tree is not binary. Node \"" << root->getLabel() << "\" has " << root->getOutEdgesCount() << " outgoing edges.";
		throw IOperator::OperatorException(msg.str());
	}

	// Check if root needs to be exchanged with one of it's children
	std::set<DecisionDiagram::Edge*> iedges = root->getInEdges();
	std::set<DecisionDiagram::Edge*> oedges = root->getOutEdges();
	int childNr = 0;
	int echildNr;
	DecisionDiagram::Node* exchangechild = NULL;
	for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
		// Assumption: child is already ordered!
		DecisionDiagram::Node* child = (*it)->getTo();

		if (dynamic_cast<DecisionDiagram::LeafNode*>(child) == NULL && getCompareAttribute(root).compare(getCompareAttribute(child)) > 0 &&	// Exchange inner nodes only
			(exchangechild == NULL || getCompareAttribute(child).compare(getCompareAttribute(exchangechild)) < 0)){				// Exchange with the smallest child
			exchangechild = child;
			echildNr = childNr;
		}
		childNr++;
	}

	if (exchangechild != NULL){
		// Root needs to be exchanged with this child

		//              root                               echild
		//        (c1)/      \(c2)       ===>       (c3)/          \(c4)
		//        sibling    echild                 root            root-copy
		//              (c3)/     \(c4)        (c1)/    \(c2)  (c1)/         \(c2)
		//          resttree1    resttree2   sibling resttree1  sibling-copy  resttree2

		// First, use human readable names for the nodes of interest
		DecisionDiagram::Node* newroot = exchangechild;
		DecisionDiagram::Node* sibling1root = root->getChild(1 - echildNr);				// Take the other child (echild's sibling)
		DecisionDiagram::Condition siblingcondition = root->getOutEdge(1 - echildNr)->getCondition();	// c1
		DecisionDiagram::Condition currentchildcondition = root->getOutEdge(echildNr)->getCondition();	// c2
		DecisionDiagram siblingCopy;									// Copy the sub-tree since we need it twice (see figure)
		siblingCopy.partialAddDecisionDiagram(&dd, sibling1root);
		siblingCopy.useUniqueLabels(&dd);

		DecisionDiagram::Node* sibling2root = dd.addDecisionDiagram(&siblingCopy);
		DecisionDiagram::Node* newrootSuccessor1 = root;						// The old root is a successor (child) of the new root
		DecisionDiagram::Node* newrootSuccessor2 = dd.addNode(dd.getUniqueLabel(root->getLabel()));	// We need the old root node twice
		DecisionDiagram::Node* resttree1root = newroot->getChild(0);					// root of resttree1
		DecisionDiagram::Condition resttree1condition = newroot->getOutEdge(0)->getCondition();		// c3
		DecisionDiagram::Node* resttree2root = newroot->getChild(1);					// root of resttree2
		DecisionDiagram::Condition resttree2condition = newroot->getOutEdge(1)->getCondition();		// c4

		// Cut out all edges from the old root to it's successors
		for (std::set<DecisionDiagram::Edge*>::iterator e = oedges.begin(); e != oedges.end(); e++) dd.removeEdge(*e);
		// Also cut ingoing edges
		for (std::set<DecisionDiagram::Edge*>::iterator e = iedges.begin(); e != iedges.end(); e++) dd.removeEdge(*e);
		// Cut out all edges from the child which becomes the new root root to it's successors
		std::set<DecisionDiagram::Edge*> newrootOutEdges = newroot->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator e = newrootOutEdges.begin(); e != newrootOutEdges.end(); e++) dd.removeEdge(*e);

		// Create new connections
		dd.addEdge(newrootSuccessor1, sibling1root, siblingcondition);					// root to sibling
		dd.addEdge(newrootSuccessor1, resttree1root, currentchildcondition);				// root to resttree1
		dd.addEdge(newrootSuccessor2, sibling2root, siblingcondition);					// root-copy to sibling-copy
		dd.addEdge(newrootSuccessor2, resttree2root, currentchildcondition);				// root-copy to resttree2

		// Sink the new root successors since sinking might goes on further
		DecisionDiagram::Node* subtree1root = sink(dd, newrootSuccessor1);				// subtree1root can be equal to newrootSuccessor1, but might be different
		DecisionDiagram::Node* subtree2root = sink(dd, newrootSuccessor2);				// subtree2root can be equal to newrootSuccessor2, but might be different

		// Redirect the edges to the new subtree roots (since they might have changed)
		dd.addEdge(newroot, subtree1root, resttree1condition);
		dd.addEdge(newroot, subtree2root, resttree2condition);

		return newroot;
	}else{
		return root;
	}
}

// orders a diagram
DecisionDiagram::Node* OpOrderBinaryDecisionTree::order(DecisionDiagram& dd, DecisionDiagram::Node* root){

	// First of all order the sub-trees (if root is a leaf, it is already sorted)
	// The overall algorithm is very similar to heap sort, even if the sinking procedure (especially redirecting the pointers between the nodes)
	// is more complicated in this case, since we must deal with conditional and unconditional edges and need to duplicate sub-trees in certain cases.
	if (dynamic_cast<DecisionDiagram::LeafNode*>(root) == NULL){

		// root is an inner node

		std::set<DecisionDiagram::Edge*> oedges = root->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
			DecisionDiagram::Node* child = (*it)->getTo();

			// redirect the edges to the new subtree roots (since they might have changed): remove old edge first, but remember it's condition
			DecisionDiagram::Condition condition = (*it)->getCondition();
			dd.removeEdge(*it);

			// order the subtree
			DecisionDiagram::Node* subtreeroot = order(dd, child);

			// restore the edge
			dd.addEdge(root, subtreeroot, condition);
		}

		// Now sink the current root node and return the new root
		return sink(dd, root);
	}else{
		// is already ordered
		return root;
	}
}

HexAnswer OpOrderBinaryDecisionTree::apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException){

	try{
		// Check arity
		if (arity != 1){
			std::stringstream msg;
			msg << "orderbinarydecisiontree is a unary operator. " << arity << " answers were passed.";
			throw IOperator::OperatorException(msg.str());
		}

		// transform all input diagrams
		HexAnswer answer;
		for (int answerSetNr = 0; answerSetNr < answers[0]->size(); answerSetNr++){
			// Construct input decision tree
			DecisionDiagram dd((*answers[0])[answerSetNr]);

			// Check preconditions
			if (!dd.isTree()){
				throw IOperator::OperatorException("orderbinarydecisiontree expects a decision tree, but the given decision diagram is not a tree.");
			}

			// Order the nodes
			order(dd, dd.getRoot());

			// Convert the final decision diagram into a hex answer
			answer.push_back(dd.toAnswerSet());
		}

		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram ide){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + ide.getMessage());
	}

}
