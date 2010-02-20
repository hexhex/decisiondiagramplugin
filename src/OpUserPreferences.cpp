#include <OpUserPreferences.h>
#include <DecisionDiagram.h>

#include <iostream>
#include <sstream>
#include <set>
#include <stdlib.h>

using namespace dlvhex::dd;

std::string OpUserPreferences::getName(){
	return "userpreferences";
}

std::string OpUserPreferences::UserPreference::UserPreference::trim(std::string s){
	int from = 0;
	int to = s.length() - 1;
	for (int i = 0; i < s.length(); i++){
		if (from == i && s[i] == ' ') from++;
		if (s[i] != ' ') to = i;
	}
	return (to > from ? s.substr(from, to - from + 1) : "");
}

OpUserPreferences::UserPreference::UserPreference(std::string definition){
	int c1 = definition.find_first_of('>');
	int c2 = definition.find_last_of('>');
	if (c1 == std::string::npos || c2 == std::string::npos || c1 == c2){
		throw InvalidUserPreference();
	}
	classP = trim(definition.substr(0, c1));
	classC = trim(definition.substr(c2 + 1));
	if (c2 - c1 > 1){
		std::string minDiffStr = definition.substr(c1 + 1, c2 - (c1 + 1));
		minDiff = atoi(minDiffStr.c_str());
		if (minDiff <= 0) throw InvalidUserPreference();
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
	for (OperatorArguments::iterator it = parameters.begin(); it != parameters.end(); it++){
		try{
			UserPreference newUp = UserPreference((*it).second);
			up.push_back(newUp);
		}catch(UserPreference::InvalidUserPreference iup){
			std::stringstream msg("");
			msg << "Line \"" << (*it).first << "\" is an invalid user preference definition.";
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
		for (int answer = 0; answer < arity; answer++){
			if (arguments[answer]->size() != 1){
				std::stringstream msg;
				msg << "userpreferences expects each answer to contain exactly one answer set. Answer " << answer << " contains " << arguments[answer]->size() << " answer-sets.";
				throw IOperator::OperatorException(msg.str());
			}
		}

		// Parse the user preferences rules
		UserPreferences userprefs = getUserPreferences(parameters);

		// Prepare a data structure for votings:
		// 	Each leaf node needs a set of string/int pairs. The string is a classification and the integer value the number of diagrams that voted for this class
		//	at this point of the diagram.
		std::map<DecisionDiagram::LeafNode*, Votings> votings;

		// Construct final decision diagram (=copy of the first input diagram for now)
		DecisionDiagram output((*arguments[0])[0]);
		if (!output.isTree()){
			throw IOperator::OperatorException("All input diagrams are expected to be trees.");
		}

		// Initialize the votings structure
		std::set<DecisionDiagram::LeafNode*> leafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator it = leafs.begin(); it != leafs.end(); it++){
			Votings uv;
			uv[(*it)->getClassification()] = 1;
			votings[*it] = uv;
		}

		// Insert all input decision diagrams
		for (int answer = 1; answer < arity; answer++){
			DecisionDiagram ddInput((*arguments[answer])[0]);
			if (!ddInput.isTree()){
				throw IOperator::OperatorException("All input diagrams are expected to be trees.");
			}
			insert(ddInput, output, votings);
		}


		// Finally, for all remaining leaf nodes, take a classification according to the user preferences
		std::set<DecisionDiagram::LeafNode*> outputLeafs = output.getLeafNodes();
		for (std::set<DecisionDiagram::LeafNode*>::iterator leafIt = outputLeafs.begin(); leafIt != outputLeafs.end(); leafIt++){
			Votings v = votings[*leafIt];
			int bestVotes = 0;
			std::string bestVotedClass("unknown");
			for (Votings::iterator vIt = v.begin(); vIt != v.end(); vIt++){
				std::string cclass = (*vIt).first;
				int cvotes = (*vIt).second;

				// check if this class is preferred over the current bestVotedClass
				//	if the current class is "unknown" (i.e. it is the default value rather than a classification founded by a diagram), it is always preferred
				if (bestVotes == 0){
					bestVotes = cvotes;
					bestVotedClass = cclass;
				}else{
					// otherwise: check if there is a user rule that gives preference to cclass
					for (UserPreferences::iterator upIt = userprefs.begin(); upIt != userprefs.end(); upIt++){
						if (	((*upIt).getPreferredClass() == cclass && (*upIt).getCmpClass() == bestVotedClass) &&
							((*upIt).getMinDiff() == (-1) || ((cvotes - bestVotes) >= (*upIt).getMinDiff()))){
							// yes: this class is preferred
							bestVotes = cvotes;
							bestVotedClass = cclass;
						}
					}
				}
			}
			// Take the best voted classification as the final one
			(*leafIt)->setClassification(bestVotedClass);
		}

		// Convert the final decision diagram into a hex answer
		HexAnswer answer;
		answer.push_back(output.toAnswerSet());
		return answer;
	}catch(DecisionDiagram::InvalidDecisionDiagram idde){
		throw IOperator::OperatorException(std::string("InvalidDecisionDiagram: ") + idde.getMessage());
	}
}
