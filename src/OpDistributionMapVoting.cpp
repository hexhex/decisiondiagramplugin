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
		 "     A      ... answer to the operator result";

	return ss.str();

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

		// Convert the final decision diagram into a hex answer
		HexAnswer answer;
		answer.push_back(diag1.toAnswerSet());
		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
