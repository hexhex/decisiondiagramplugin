#include <OpUserPreferences.h>
#include <DecisionDiagram.h>
#include <StringHelper.h>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <set>
#include <stdlib.h>

using namespace dlvhex::dd::util;
using namespace dlvhex::dd::plugin;

std::string OpUserPreferences::getName(){
	return "userpreferences";
}

OpUserPreferences::UserPreference::UserPreference(std::string definition){
	// extract compared classes
	int c1 = definition.find_first_of('>');
	int c2 = definition.find_last_of('>');
	if (c1 == std::string::npos || c2 == std::string::npos || c1 == c2){
		throw InvalidUserPreference();
	}
	classP = definition.substr(0, c1);
	classC = definition.substr(c2 + 1);
	boost::trim(classP);
	boost::trim(classC);

	// extract comparison operation
	if (c2 - c1 > 1){
		std::string minDiffStr = definition.substr(c1 + 1, c2 - (c1 + 1));
		try{
			minDiff = StringHelper::atoi(minDiffStr.c_str());
			if (minDiff <= 0) throw InvalidUserPreference();
		}catch(StringHelper::NotContainedException){
			// invalid number
			throw InvalidUserPreference();
		}
	}else{
		minDiff = -1;
	}
}

std::string OpUserPreferences::UserPreference::getPreferredClass(){
	return classP;
}

std::string OpUserPreferences::UserPreference::getCmpClass(){
	return classC;
}

int OpUserPreferences::UserPreference::getMinDiff(){
	return minDiff;
}

OpUserPreferences::UserPreferences OpUserPreferences::getUserPreferences(OperatorArguments& parameters){
	UserPreferences up;
	// extract all user preference rules
	for (OperatorArguments::iterator it = parameters.begin(); it != parameters.end(); it++){
		try{
			// try to parse it
			UserPreference newUp = UserPreference((*it).second);
			up.push_back(newUp);
		}catch(UserPreference::InvalidUserPreference iup){
			std::stringstream msg("");
			msg << "Line \"" << (*it).second << "\" is an invalid user preference definition.";
			throw OperatorException(msg.str());
		}
	}
	return up;
}

HexAnswer OpUserPreferences::apply(int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters) throw (OperatorException){

	try{
		// Check arity
		if (arity < 1){
			std::stringstream msg;
			msg << "userpreferences expects at least one diagram as arguments.";
			throw IOperator::OperatorException(msg.str());
		}

		// each answer must contain one answer-set
		for (int answer = 0; answer < arity; answer++){
			if (arguments[answer]->size() != 1){
				std::stringstream msg;
				msg << "userpreferences expects each answer to contain exactly one answer set. Answer " << answer << " contains " << arguments[answer]->size() << " answer-sets.";
				throw IOperator::OperatorException(msg.str());
			}
		}

		// Parse the user preferences rules
		UserPreferences userprefs = getUserPreferences(parameters);

		// Construct final decision diagram (=copy of the first input diagram for now)
		DecisionDiagram output((*arguments[0])[0]);
		if (!output.isTree()){
			throw IOperator::OperatorException("All input diagrams are expected to be trees.");
		}

		// Initialize the votings structure
		std::set<DecisionDiagram::LeafNode*> leafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator it = leafs.begin(); it != leafs.end(); it++){
			Votes* votes = new Votes();
			votes->v[(*it)->getClassification()] = 1;
			(*it)->setData(votes);
		}

		// Insert all input decision diagrams
		for (int answer = 1; answer < arity; answer++){
			DecisionDiagram ddInput((*arguments[answer])[0]);
			if (!ddInput.isTree()){
				throw IOperator::OperatorException("All input diagrams are expected to be trees.");
			}
			insert(ddInput, output);
		}


		// Finally, for all remaining leaf nodes, take a classification according to the user preferences
		std::set<DecisionDiagram::LeafNode*> outputLeafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator leafIt = outputLeafs.begin(); leafIt != outputLeafs.end(); leafIt++){
			Votes* v = dynamic_cast<Votes*>((*leafIt)->getData());
			int bestVotes = 0;
			std::string bestVotedClass("unknown");

			// go through all classes in this leaf node and take the highest voted class as the first guess
			for (Votings::iterator vIt = v->v.begin(); vIt != v->v.end(); vIt++){
				std::string cclass = (*vIt).first;
				int cvotes = (*vIt).second;
				if (cvotes > bestVotes || cvotes == bestVotes && cclass.compare(bestVotedClass) < 0){
					bestVotedClass = cclass;
					bestVotes = cvotes;
				}
			}

			// go through all preference rules
			for (UserPreferences::iterator upIt = userprefs.begin(); upIt != userprefs.end(); upIt++){
				// check if this rule is applicable
				if (	((*upIt).getCmpClass() == bestVotedClass) &&
					(v->v.find((*upIt).getPreferredClass()) != v->v.end() && (*upIt).getMinDiff() == (-1) ||	// either the preferred class is containted in the leaf node and is preferred in any case
					((v->v[(*upIt).getPreferredClass()] - bestVotes) >= (*upIt).getMinDiff()))){			// or it has at least "n" votes more than the cmp class
					// yes: this class is preferred
					bestVotedClass = (*upIt).getPreferredClass();
					bestVotes = v->v[bestVotedClass];
				}
			}

			// Take the best voted classification as the final one
			(*leafIt)->setClassification(bestVotedClass);
			delete v;
		}

		// Convert the final decision diagram into a hex answer
		HexAnswer answer;
		answer.push_back(output.toAnswerSet());
		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
