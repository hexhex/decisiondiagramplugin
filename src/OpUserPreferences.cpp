#include <OpUserPreferences.h>
#include <DecisionDiagram.h>

#include <sstream>
#include <set>

using namespace dlvhex::dd;

std::string OpUserPreferences::getName(){
	return "userpreferences";
}

HexAnswer OpUserPreferences::apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters){
}
