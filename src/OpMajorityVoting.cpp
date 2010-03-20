#include <OpMajorityVoting.h>
#include <DecisionDiagram.h>

#include <iostream>
#include <sstream>
#include <set>

using namespace dlvhex::dd::util;
using namespace dlvhex::dd::plugin;

std::string OpMajorityVoting::getName(){
	return "majorityvoting";
}

void OpMajorityVoting::insert(DecisionDiagram& input, DecisionDiagram& output){

	// Merge the decision diagrams
	// Extract all leaf nodes of the intermediate result
	std::set<DecisionDiagram::LeafNode*> outputLeafs = output.getLeafNodes();

	// Make a copy of the new input diagram for each leaf node
	for (std::set<DecisionDiagram::LeafNode*>::iterator formerLeafIt = outputLeafs.begin(); formerLeafIt != outputLeafs.end(); formerLeafIt++){
		Votes* formerVotes = dynamic_cast<Votes*>((*formerLeafIt)->getData());

		// Make a copy of the input diagram
		DecisionDiagram inputCopy(input);

		// Rename nodes if necessary
		inputCopy.useUniqueLabels(&output);

		// Add it to the output diagram
		DecisionDiagram::Node* ddInputRoot = output.addDecisionDiagram(&inputCopy);

		// Adjust the votings structure
		//	First, add an entry for each new leaf node
		std::set<DecisionDiagram::LeafNode*> newoutputLeafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator newLeafIt = newoutputLeafs.begin(); newLeafIt != newoutputLeafs.end(); newLeafIt++){
			// Check if this is a new leaf node
			if ((*newLeafIt)->getData() == NULL){
				// The new entry is equal to the entry of the former leaf, except that the counter for the classification of the new leaf is incremented by 1
				Votes* newVotes = new Votes();
				newVotes->v = formerVotes->v;
				if (newVotes->v.find((*newLeafIt)->getClassification()) == newVotes->v.end()){
					newVotes->v[(*newLeafIt)->getClassification()] = 1;
				}else{
					newVotes->v[(*newLeafIt)->getClassification()]++;
				}
				(*newLeafIt)->setData(newVotes);
			}
		}

		//	Remove the votings for the former leaf node
		delete formerVotes;

		// Redirect all in-edges to the former leaf node to the root element of ddInput
		std::set<DecisionDiagram::Edge*> inEdges = (*formerLeafIt)->getInEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator inIt = inEdges.begin(); inIt != inEdges.end(); inIt++){
			// Insert a new edge
			output.addEdge((*inIt)->getFrom(), ddInputRoot, (*inIt)->getCondition());
			// Remove the old edge
			output.removeEdge(*inIt);
		}

		// Remove the former leaf node
		output.removeNode(*formerLeafIt);

	}
}

HexAnswer OpMajorityVoting::apply(int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters) throw (OperatorException){

	try{
		// Check arity
		if (arity < 1){
			std::stringstream msg;
			msg << "majorityvoting expects at least one diagram as arguments.";
			throw IOperator::OperatorException(msg.str());
		}
		for (int answer = 0; answer < arity; answer++){
			if (arguments[answer]->size() != 1){
				std::stringstream msg;
				msg << "majorityvoting expects each answer to contain exactly one answer set. Answer " << answer << " contains " << arguments[answer]->size() << " answer-sets.";
				throw IOperator::OperatorException(msg.str());
			}
		}

		// Construct final decision diagram (=copy of the first input diagram for now)
		DecisionDiagram output((*arguments[0])[0]);
		if (!output.isTree()){
			throw IOperator::OperatorException("All input diagrams are expected to be trees.");
		}

		// Initialize the votings structure for the leafs of the startup diagram
		std::set<DecisionDiagram::LeafNode*> leafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator it = leafs.begin(); it != leafs.end(); it++){
			Votes* votings = new Votes();
			votings->v[(*it)->getClassification()] = 1;
			(*it)->setData(votings);
		}

		// Insert all input decision diagrams
		for (int answer = 1; answer < arity; answer++){
			DecisionDiagram ddInput((*arguments[answer])[0]);
			if (!ddInput.isTree()){
				throw IOperator::OperatorException("All input diagrams are expected to be trees.");
			}
			insert(ddInput, output);
		}

		// Finally, for all remaining leaf nodes, take the classification with the highest votes
		std::set<DecisionDiagram::LeafNode*> outputLeafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator leafIt = outputLeafs.begin(); leafIt != outputLeafs.end(); leafIt++){
			int highestVotes = 0;
			std::string highestVotedClass("unknown");

			// search for the highest voted classification
			Votes* votes = dynamic_cast<Votes*>((*leafIt)->getData());
			for (Votings::iterator vIt = votes->v.begin(); vIt != votes->v.end(); vIt++){
				if ((*vIt).second > highestVotes || ((*vIt).second == highestVotes && (*vIt).first.compare(highestVotedClass) < 0)){
					highestVotes = (*vIt).second;
					highestVotedClass = (*vIt).first;
				}
			}
			delete (*leafIt)->getData();

			// Take the highest voted classification as the final one
			(*leafIt)->setClassification(highestVotedClass);
		}

		// Convert the final decision diagram into a hex answer
		HexAnswer answer;
		answer.push_back(output.toAnswerSet());
		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
