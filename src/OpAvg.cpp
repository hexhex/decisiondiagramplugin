#include <OpAvg.h>
#include <DecisionDiagram.h>
#include <StringHelper.h>

#include <sstream>
#include <set>

using namespace dlvhex::dd::util;
using namespace dlvhex::dd::plugin;

std::string OpAvg::getName(){
	return "avg";
}

std::string OpAvg::getInfo(){
	std::stringstream ss;
	ss <<	"   avg" << std::endl <<
		"   ---"  << std::endl << std::endl <<
		"This class implements the average operator. It assumes that 2 answers are passed to the operator (binary operator) with one ordered binary diagram tree each." << std::endl <<
	 	"The result will be another ordered binary diagram tree where all constants in range queries are averaged if the input decision trees differ." << std::endl <<
	 	"Usage:" << std::endl <<
		"   &operator[\"average\", DD, K](A)" << std::endl <<
		"     DD     ... predicate with handles to exactly 2 answers containing one ordered binary decision tree each" << std::endl <<
	 	"     A      ... answer to the operator result";
	return ss.str();

}

std::set<std::string> OpAvg::getRecognizedParameters(){
	std::set<std::string> list;
	return list;
}

// retrieves the condition tested in a node; in strict sense, conditions are assigned to edges and not to nodes. but if we have only one conditional edge that origins in a certain node,
// we can also consider the condition to be assigned to that node
DecisionDiagram::Condition OpAvg::getCondition(DecisionDiagram::Node* node){
	// Search for the requested condition in the outgoing edges of this node
	DecisionDiagram::Condition c("", "", DecisionDiagram::Condition::else_);
	bool cFound = false;
	std::set<DecisionDiagram::Edge*> oedges = node->getOutEdges();
	for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
		DecisionDiagram::Condition condition = (*it)->getCondition();
		if (condition.getOperation() != DecisionDiagram::Condition::else_){
			if (cFound){
				std::stringstream msg;
				msg << "Node \"" << node->getLabel() << "\" has more than one outgoing conditional edge, but average expects the input diagrams to be binary with exactly one else edge";
				throw IOperator::OperatorException(msg.str());
			}
			c = condition;
			cFound = true;
		}
	}
	if (!cFound){
		std::stringstream msg;
		msg << "Node \"" << node->getLabel() << "\" has no outgoing conditional edge";
		throw IOperator::OperatorException(msg.str());
	}
	return c;
}

// merges two leaf nodes
DecisionDiagram::Node* OpAvg::averageLeafs(DecisionDiagram& result, DecisionDiagram* dd1, DecisionDiagram::LeafNode* leaf1, DecisionDiagram* dd2, DecisionDiagram::LeafNode* leaf2){
	// Check if they coincide
	if (leaf1->getClassification() == leaf2->getClassification()){
		// Yes: Add the same classification to the final result
		return result.addLeafNode(result.getUniqueLabel(leaf1->getLabel()), leaf1->getClassification());
	}else{
		// No: Classifications are contradictory
		return result.addLeafNode(result.getUniqueLabel(leaf1->getLabel()), std::string("unknown"));
	}
}

// merges two inner nodes
DecisionDiagram::Node* OpAvg::averageInner(DecisionDiagram& result, DecisionDiagram* dd1, DecisionDiagram::Node* inner1, DecisionDiagram* dd2, DecisionDiagram::Node* inner2){
	// No: Both are inner nodes

	DecisionDiagram::Condition c1 = getCondition(inner1);
	DecisionDiagram::Condition c2 = getCondition(inner2);

	// Check if n1 and n2 both query the same variable
	if (c1.getOperand1() == c2.getOperand1()){
		// Average

		// Both nodes request the same variable

		// Merge the sub-trees. The else-sides and the conditional edges are merged accordingly.
		std::set<DecisionDiagram::Edge*> n1edges = inner1->getOutEdges();
		std::set<DecisionDiagram::Edge*> n2edges = inner2->getOutEdges();
		DecisionDiagram::Node* else_subtree_1;
		DecisionDiagram::Node* conditional_subtree_1;
		DecisionDiagram::Node* else_subtree_2;
		DecisionDiagram::Node* conditional_subtree_2;
		for (std::set<DecisionDiagram::Edge*>::iterator it = n1edges.begin(); it != n1edges.end(); it++)
			if ((*it)->getCondition().getOperation() == DecisionDiagram::Condition::else_)	else_subtree_1 = (*it)->getTo();
			else										conditional_subtree_1 = (*it)->getTo();
		for (std::set<DecisionDiagram::Edge*>::iterator it = n2edges.begin(); it != n2edges.end(); it++)
			if ((*it)->getCondition().getOperation() == DecisionDiagram::Condition::else_)	else_subtree_2 = (*it)->getTo();
			else										conditional_subtree_2 = (*it)->getTo();

		// merge the conditional and the else subtree independently
		DecisionDiagram::Node* else_subtree = average(result, dd1, else_subtree_1, dd2, else_subtree_2);
		DecisionDiagram::Node* conditional_subtree = average(result, dd1, conditional_subtree_1, dd2, conditional_subtree_2);

		// Now the conditions must be merged
		double o1 = StringHelper::atof(StringHelper::unquote(c1.getOperand2()).c_str());
		double o2 = StringHelper::atof(StringHelper::unquote(c2.getOperand2()).c_str());
		double o3 = (o1 + o2) / 2;
		DecisionDiagram::Condition merged_condition = DecisionDiagram::Condition(c1.getOperand1(), StringHelper::toString(o3), c1.getOperation());

		// If both second operands are numbers and the condition operators are equal, we just take the average
		DecisionDiagram::Node* root = result.addNode(result.getUniqueLabel(inner1->getLabel()));

		// Finally, connect the root with it's subtrees
		result.addEdge(root, conditional_subtree, merged_condition);
		result.addElseEdge(root, else_subtree);

		return root;
	}else{
		// No: Since the input decision diagrams (trees) can savely assumed to be ordered,
		// we know for sure that the lexically smaller one of the compared attributes does not occur in the other decision diagram
		// Just merge each child of the decision node with the lexically smaller attribute with the other decision diagram. The larger one may occurs somewhere below.

		// Example:
		//
		//    [A]               [B]
		//   /   \             /   \
		// ...   ...         ...   ...
		//  I     II
		//
		// We know, that A is never requested in the right decision diagram. Since B is already requested, and the diagram is ordered, there is no chance to request A below.
		// So we savely merge the right decision diagram with the children of the left diagram (I and II), since B can occur there.

		DecisionDiagram* smallerDD;
		DecisionDiagram* largerDD;
		DecisionDiagram::Node* smallerN;
		DecisionDiagram::Node* largerN;
		if (c1.getOperand1().compare(c2.getOperand1()) < 0){	// c1 is lexically smaller than c2
			smallerDD = dd1;
			smallerN = inner1;
			largerDD = dd2;
			largerN = inner2;
		}else{							// c2 is lexically smaller than c1
			smallerDD = dd2;
			smallerN = inner2;
			largerDD = dd1;
			largerN = inner1;
		}

		// Insert the smaller node into the final result
		DecisionDiagram::Node* root = result.addNode(result.getUniqueLabel(smallerN->getLabel()));

		// Merge the larger one with all children of the smaller node
		std::set<DecisionDiagram::Edge*> smaller_oedges = smallerN->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator it = smaller_oedges.begin(); it != smaller_oedges.end(); it++){
			DecisionDiagram::Node* subdiagramRoot = average(result, smallerDD, (*it)->getTo(), largerDD, largerN);

			// Connect the currently inserted node with the sub-diagram
			result.addEdge(root, subdiagramRoot, (*it)->getCondition());
		}

		return root;
	}
}

// merges an inner with a leaf node
DecisionDiagram::Node* OpAvg::averageInnerLeaf(DecisionDiagram& result, DecisionDiagram* innerdiag, DecisionDiagram::Node* inner, DecisionDiagram* leafdiag, DecisionDiagram::LeafNode* leaf){
	// Insert the non-leaf node into the final result
	DecisionDiagram::Node* root = result.addNode(result.getUniqueLabel(inner->getLabel()));

	// Further pass the request to all children of the non-leaf
	std::set<DecisionDiagram::Edge*> oedges = inner->getOutEdges();
	for (std::set<DecisionDiagram::Edge*>::iterator it = oedges.begin(); it != oedges.end(); it++){
		DecisionDiagram::Node* subdiagramRoot = average(result, innerdiag, (*it)->getTo(), leafdiag, leaf);

		// Connect the currently inserted node with the sub-diagram
		result.addEdge(root, subdiagramRoot, (*it)->getCondition());
	}

	return root;
}

DecisionDiagram::Node* OpAvg::average(DecisionDiagram& result, DecisionDiagram* dd1, DecisionDiagram::Node* n1, DecisionDiagram* dd2, DecisionDiagram::Node* n2){

	// Check if one or both of the currently processed nodes is a leaf
	if (dynamic_cast<DecisionDiagram::LeafNode*>(n1) != NULL && dynamic_cast<DecisionDiagram::LeafNode*>(n2) != NULL){
		// Yes: Both are leafs

		DecisionDiagram::LeafNode* leaf1 = dynamic_cast<DecisionDiagram::LeafNode*>(n1);
		DecisionDiagram::LeafNode* leaf2 = dynamic_cast<DecisionDiagram::LeafNode*>(n2);

		return averageLeafs(result, dd1, leaf1, dd2, leaf2);
	}else if (dynamic_cast<DecisionDiagram::LeafNode*>(n1) != NULL || dynamic_cast<DecisionDiagram::LeafNode*>(n2) != NULL){
		// Yes: One is leaf, one is non-leaf

		DecisionDiagram::Node* nonleaf = (dynamic_cast<DecisionDiagram::LeafNode*>(n1) != NULL ? n2 : n1);
		DecisionDiagram* nonleafdiag = (dynamic_cast<DecisionDiagram::LeafNode*>(n1) != NULL ? dd2 : dd1);
		DecisionDiagram::LeafNode* leaf = dynamic_cast<DecisionDiagram::LeafNode*>(nonleaf == n1 ? n2 : n1);
		DecisionDiagram* leafdiag = nonleafdiag == dd1 ? dd2 : dd1;

		return averageInnerLeaf(result, nonleafdiag, nonleaf, leafdiag, leaf);
	}else{
		// both are inner nodes
		return averageInner(result, dd1, n1, dd2, n2);
	}
}

HexAnswer OpAvg::apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException){
	try{
		// Check arity
		if (arity != 2){
			std::stringstream msg;
			msg << "average is a binary (2-ary) operator. " << arity << " answers were passed.";
			throw IOperator::OperatorException(msg.str());
		}
		if (answers[0]->size() != 1 || answers[1]->size() != 1){
			std::stringstream msg;
			msg << "average expects each answer to contain exactly one answer set.";
			throw IOperator::OperatorException(msg.str());
		}

		// Construct input decision diagrams
		DecisionDiagram dd1((*answers[0])[0]);
		DecisionDiagram dd2((*answers[1])[0]);

		// check if inputs are trees
		if (!dd1.isTree() || !dd2.isTree()){
			throw IOperator::OperatorException("average expects each of it's input diagrams to be an ordered tree.");
		}

		// Merge the diagrams
		DecisionDiagram result;
		average(result, &dd1, dd1.getRoot(), &dd2, dd2.getRoot());

		// Convert the final decision diagram into a hex answer
		HexAnswer answer;
		answer.push_back(result.toAnswerSet());
		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("average: ") + idde.getMessage());
	}
}
