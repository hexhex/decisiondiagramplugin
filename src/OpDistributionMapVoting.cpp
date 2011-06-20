#include <OpDistributionMapVoting.h>
#include <DecisionDiagram.h>

#include <StringHelper.h>

#include <sstream>
#include <set>

using namespace dlvhex::dd::util;
using namespace dlvhex::dd::plugin;

std::string OpDistributionMapVoting::getName(){
	return "distributionmapvoting";
}

std::string OpDistributionMapVoting::getInfo(){
	std::stringstream ss;
	ss <<	"   distributionmapvoting" << std::endl <<
		"   ---------------------"  << std::endl << std::endl <<
		 "This class implements an operator that merged diagrams according to the distribution maps in their leaf nodes." << std::endl <<
		 "It assumes that 2 answers are passed to the operator (binary operator) with one decision diagram each. The leaf nodes do not only need to contain the classification, but also" << std::endl <<
		 "the distribution of training examples over the classes which ended in this leaf. For instance, if 4 training examples belong to class1 and 2 to class2, the classification is" << std::endl <<
		 "\"class1\" and the distribution map is \"class1:4,class2:2\"." << std::endl <<
		 "The syntax is as follows:" << std::endl <<
		 " 	\"classification {c1:n1,c2:n2,...,cn:nm}\"" << std::endl <<
		 "The result will be another decision diagram which combines diagrams by inserting the second into all leaf nodes of the first one, and recomputing the classification according" << std::endl <<
		 "to the (combined) distribution map." << std::endl <<
		 "Usage:" << std::endl <<
		 "   &operator[\"majorityvoting\", DD, K](A)" << std::endl <<
		 "     DD     ... predicate with indices 0-1 and handles to exactly 2 answers containing one decision diagram each" << std::endl <<
		 "     A      ... answer to the operator result" << std::endl <<
		 "     K      ... may specifies an epsilon value \"eps=P\" where P is a percentage value;" << std::endl <<
		 "                if a class frequency is greater than P*max, also this alternative diagram will be produces";
	return ss.str();

}

std::set<std::string> OpDistributionMapVoting::getRecognizedParameters(){
	std::set<std::string> list;
	list.insert("eps");
	return list;
}

void OpDistributionMapVoting::insert(DecisionDiagram& input, DecisionDiagram& output){

	// Merge the decision diagrams
	// Extract all leaf nodes of dd1
	std::set<DecisionDiagram::LeafNode*> outputLeafs = output.getLeafNodes();

	// Make a copy of ddInput for each leaf node
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
				try{
					// The new entry for a certain classification is equal to the sum of the former entries
					Votings uv = StringHelper::extractDistribution((*newLeafIt)->getClassification());
					Votes* newVotes = new Votes();
					newVotes->v = formerVotes->v;
					for (Votings::iterator vIt = newVotes->v.begin(); vIt != newVotes->v.end();  vIt++){
						newVotes->v[vIt->first] +=uv[vIt->first]; 
					}
					(*newLeafIt)->setData(newVotes);
				}catch(StringHelper::NotContainedException nce){
					throw OperatorException(std::string("Leaf node with label \"") + (*newLeafIt)->getClassification() + std::string("\" does not contain a distribution map"));
				}
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

HexAnswer OpDistributionMapVoting::apply(int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters) throw (OperatorException){

	// Process parameters
	eps = 1.0f;
	for (OperatorArguments::iterator it = parameters.begin(); it != parameters.end(); ++it){
		if (it->first == std::string("eps")){
			eps = ((float)atoi(it->second.c_str()) / 100);
		}
	}

	try{
		// Check arity
		if (arity != 2){
			std::stringstream msg;
			msg << "distributionmapvoting expects exactly two diagrams as arguments.";
			throw IOperator::OperatorException(msg.str());
		}
		for (int answer = 0; answer < arity; answer++){
			if (arguments[answer]->size() != 1){
				std::stringstream msg;
				msg << "distributionmapvoting expects each answer to contain exactly one answer set. Answer " << answer << " contains " << arguments[answer]->size() << " answer-sets.";
				throw IOperator::OperatorException(msg.str());
			}
		}

		// Prepare a data structure for votings:
		// 	Each leaf node needs a set of string/int pairs. The string is a classification and the integer value the number of training examples that ended in this leaf node and
		//	voted for this class
		std::map<DecisionDiagram::LeafNode*, Votings> votings;

		// Construct both decision diagrams
		DecisionDiagram diag1((*arguments[0])[0]);
		DecisionDiagram diag2((*arguments[1])[0]);
		if (!diag1.isTree() || !diag2.isTree()){
			throw IOperator::OperatorException("All input diagrams are expected to be trees.");
		}

		// Initialize the votings structure
		std::set<DecisionDiagram::LeafNode*> leafs = diag1.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator it = leafs.begin(); it != leafs.end(); it++){
			try{
				Votes* v = new Votes();
				v->v = StringHelper::extractDistribution((*it)->getClassification());
				(*it)->setData(v);
			}catch(StringHelper::NotContainedException nce){
				throw OperatorException(std::string("Leaf node with label \"") + (*it)->getClassification() + std::string("\" does not contain a distribution map"));
			}
		}

		// Insert the second decision diagram into the first one
		insert(diag2, diag1);

		// Set all class labels of the final diagram to empty "" (=not yet computed)
		std::set<DecisionDiagram::LeafNode*> outputLeafs = diag1.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator leafIt = outputLeafs.begin(); leafIt != outputLeafs.end(); leafIt++){
			(*leafIt)->setClassification("");
		}

		// Now extract the final diagrams, respecting the eps value
		std::vector<DecisionDiagram> result = extractDiagrams(eps, diag1);
/*
		// Finally, for all remaining leaf nodes, take the classification with the highest number of votes
		std::set<DecisionDiagram::LeafNode*> outputLeafs = diag1.getLeafNodes();
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

			// Take the highest voted classification as the final one
			(*leafIt)->setClassification(highestVotedClass + StringHelper::encodeDistributionMap(votes->v));
			delete votes;
		}
*/

		// Convert the final decision diagrams into hex answers
		HexAnswer answer;
		for (std::vector<DecisionDiagram>::iterator it = result.begin(); it != result.end(); ++it){
			answer.push_back(it->toAnswerSet());
		}
		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
//#include <iostream>
std::vector<DecisionDiagram> OpDistributionMapVoting::extractDiagrams(float eps, DecisionDiagram& diag){

	std::vector<DecisionDiagram> result;

	// Go through all leaf nodes
//int d = 0;
	std::set<DecisionDiagram::LeafNode*> leafs = diag.getLeafNodes();
	for (std::set<DecisionDiagram::LeafNode*>::iterator leafIt = leafs.begin(); leafIt != leafs.end(); leafIt++){
//d++;
		// make sure that this terminates: go to the first unprocessed leaf node
		if ((*leafIt)->getClassification() == std::string("")){
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

			// for each class label with >= eps * highestVotes create a copy of the diagram
//std::cout << votes->v.size() <<  " " ;
			for (Votings::iterator vIt = votes->v.begin(); vIt != votes->v.end(); vIt++){
				if ((*vIt).second > (int)((float)highestVotes * eps) || ((*vIt).second == highestVotes && eps == 1.0f)){
//std::cout << "+";
//for (int j = 0; j < d; j++) std::cout << "     ";
//std::cout << "Try " << (*vIt).first << std::endl;
					(*leafIt)->setClassification((*vIt).first + StringHelper::encodeDistributionMap(votes->v));

					// Recursively compute the outcome for the other leaf nodes
					std::vector<DecisionDiagram> subresult = extractDiagrams(eps, diag);
					result.insert(result.end(), subresult.begin(), subresult.end());
//std::cout << "#" << subresult.size() << " " ;
				}
			}
			// Backtrack: Restore "unprocessedness" of the current leaf node
			(*leafIt)->setClassification("");

			// finally we have processed all leaf nodes
			return result;
		}
	}

	// no more unprocessed diagrams -> done
	result.push_back(diag);
	return result;
}
