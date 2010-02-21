#include <OpSimplify.h>
#include <DecisionDiagram.h>

#include <iostream>
#include <sstream>
#include <set>

using namespace dlvhex::dd;

std::string OpSimplify::getName(){
	return "simplify";
}

// removes the connective component that "n" is part of
// this method has a built-in security check: it won't remove components that are reachable from the root
void OpSimplify::removeConnectiveComponent(DecisionDiagram& dd, DecisionDiagram::Node* n){
	// check if n is reachible from the root
	if (dd.containsPath(dd.getRoot(), n)) return;

	std::set<DecisionDiagram::Edge*> outEdges = n->getOutEdges();
	std::set<DecisionDiagram::Edge*> inEdges = n->getInEdges();

	std::vector<DecisionDiagram::Node*> neighbors;

	// remove all neighbor nodes
	for (std::set<DecisionDiagram::Edge*>::iterator it = outEdges.begin(); it != outEdges.end(); it++){
		DecisionDiagram::Node* neighbor = (*it)->getTo();
		// first remove the edge to the neighbor (to avoid infinite recursion)
		dd.removeEdge(*it);
		neighbors.push_back(neighbor);
	}

	for (std::set<DecisionDiagram::Edge*>::iterator it = inEdges.begin(); it != inEdges.end(); it++){
		DecisionDiagram::Node* neighbor = (*it)->getFrom();
		// first remove the edge to the neighbor (to avoid infinite recursion)
		dd.removeEdge(*it);
		neighbors.push_back(neighbor);
	}

	// now remove the neighboring connective component recursivly
	for (std::vector<DecisionDiagram::Node*>::iterator it = neighbors.begin(); it != neighbors.end(); it++){
		removeConnectiveComponent(dd, *it);
	}

	// remove the node itself
	try{
		dd.removeNode(n);
	}catch(DecisionDiagram::InvalidDecisionDiagram idd){
		// can be thrown in case that a node is entered twice due to two nodes with a common ancestor or successor
	}
}

// reduces the subgraph with root "n" to a single leaf node if this is possible, i.e. if all branches lead to the same final classification
// the return value will be a pointer to the new root node (a leaf node), which is equivalent to "n" if the subgraph could not be reduced
DecisionDiagram::Node* OpSimplify::reduceSubgraph(DecisionDiagram& dd, DecisionDiagram::Node* n){

	// leafs can never be reduced (since they are already minimal)
	if (dynamic_cast<DecisionDiagram::LeafNode*>(n)){
		return n;
	}else{
		// reduce all subgraphs
		std::set<DecisionDiagram::Edge*> outedges = n->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator outEdgeIt = outedges.begin(); outEdgeIt != outedges.end(); outEdgeIt++){
			DecisionDiagram::Node* newSubroot = reduceSubgraph(dd, (*outEdgeIt)->getTo());
			if (newSubroot != n){

				// add new edge
				dd.addEdge(n, newSubroot, (*outEdgeIt)->getCondition());

				// remember the former to-node of this edge
				DecisionDiagram::Node* removeComponent = (*outEdgeIt)->getTo();

				// redirect the inedge from n to newSubroot
				dd.removeEdge(*outEdgeIt);

				// if this was part of the last path from the root to this connective component (i.e. the edge's to-node is now unreachible), it can be removed
				if (!dd.containsPath(dd.getRoot(), removeComponent)){
					removeConnectiveComponent(dd, removeComponent);
				}
			}
		}

		outedges = n->getOutEdges();
		DecisionDiagram::Node* commonSubgraph;
		for (std::set<DecisionDiagram::Edge*>::iterator outEdgeIt = outedges.begin(); outEdgeIt != outedges.end(); outEdgeIt++){
			DecisionDiagram::Node* sn = (*outEdgeIt)->getTo();
			if (outEdgeIt == outedges.begin()){
				commonSubgraph = sn;
			}else if(*sn != *commonSubgraph){
				// at least two different subgraphs were found: cannot reduce
				return n;
			}
		}
		// all classifications are equal: the node can be reduced
		return commonSubgraph;
	}
}

DecisionDiagram OpSimplify::simplify(DecisionDiagram dd){

	// -------------------- strategy 1: remove unnecessary conditions  --------------------
	// conditions are unnecessary if all branches lead to the same final classification
	std::set<DecisionDiagram::Edge*> outedges = dd.getRoot()->getOutEdges();
	DecisionDiagram::Node* oldRoot = dd.getRoot();
	DecisionDiagram::Node* newRoot = reduceSubgraph(dd, oldRoot);
	if (newRoot != oldRoot){
		dd.setRoot(newRoot);
		removeConnectiveComponent(dd, oldRoot);
	}

	bool restart = true;
	while (restart){
		restart = false;
		std::set<DecisionDiagram::Node*> nodes = dd.getNodes();

		// -------------------- strategy 2: combine equivalent subdiagrams --------------------
		// for each node of the diagram, check if there is another node that is equal to it
		for (std::set<DecisionDiagram::Node*>::iterator it1 = nodes.begin(); it1 != nodes.end() && !restart; it1++){

			for (std::set<DecisionDiagram::Node*>::iterator it2 = nodes.begin(); it2 != nodes.end() && !restart; it2++){
				// iterators must point to different nodes that are semantically equivalent
				if ((*it1) != (*it2) && (**it1) == (**it2)){
					// replace one of the nodes by the other one, that is, redirect all in-edges into *it2 to *it1
					std::set<DecisionDiagram::Edge*> inedges = (*it1)->getInEdges();
					for (std::set<DecisionDiagram::Edge*>::iterator inEdgeIt = inedges.begin(); inEdgeIt != inedges.end(); inEdgeIt++){
						// Add new edge
						dd.addEdge((*inEdgeIt)->getFrom(), *it2, (*inEdgeIt)->getCondition());

						// remember the former to-node of this edge
						DecisionDiagram::Node* removeComponent = (*inEdgeIt)->getTo();

						// Remove the old edge
						dd.removeEdge(*inEdgeIt);

						// if this was part of the last path from the root to this connective component (i.e. the edge's to-node is now unreachible), it can be removed
						if (!dd.containsPath(dd.getRoot(), removeComponent)){
							removeConnectiveComponent(dd, removeComponent);
							// restart outer loops since essential graph structues have changed and the iterators are invalid now
							restart = true;
							break;
						}
					}
				}
			}
		}
	}

	return dd;
}

HexAnswer OpSimplify::apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException){

	try{
		// Check arity
		if (arity != 1){
			std::stringstream msg;
			msg << " " << arity << " answers were passed.";
			throw IOperator::OperatorException(msg.str());
		}

		HexAnswer output;
		for (int answer = 0; answer < arity; answer++){
			for (int answerset = 0; answerset < answers[answer]->size(); answerset++){
				DecisionDiagram dd((*answers[answer])[answerset]);
				output.push_back(simplify(dd).toAnswerSet());
			}
		}

		return output;
	}catch(DecisionDiagram::InvalidDecisionDiagram ide){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + ide.getMessage());
	}

}
