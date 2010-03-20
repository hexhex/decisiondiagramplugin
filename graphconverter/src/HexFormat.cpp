#include <HexFormat.h>
#include <StringHelper.h>

#include <dlvhex/HexParserDriver.h>
#include <dlvhex/DLVresultParserDriver.h>
#include <dlvhex/PrintVisitor.h>
#include <dlvhex/Program.h>

#include <iostream>
#include <string>
#include <sstream>

using namespace dlvhex::dd::tools::graphconverter;

std::string HexFormat::getName(){
	return "hexprogram";
}

std::string HexFormat::getNameAbbr(){
	return "hex";
}

DecisionDiagram* HexFormat::read() throw (DecisionDiagram::InvalidDecisionDiagram){
	// parse the hex program (a set of facts)
	HexParserDriver hexparser;
	Program prog;
	AtomSet atoms;
	hexparser.parse(std::cin, prog, atoms);

	// check if the program consists of facts only
	for (Program::iterator rule = prog.begin(); rule != prog.end(); ++rule){
		throw DecisionDiagram::InvalidDecisionDiagram("Error: A rule was found. The input program must not contain rules, but facts only.");
	}

	// check if the program contains only legal predicates with legal arities
	for (AtomSet::const_iterator atom = atoms.begin(); atom != atoms.end(); ++atom){
		if (atom->getPredicate().getString() == std::string("root") && atom->getArity() == 1) continue;
		if (atom->getPredicate().getString() == std::string("innernode") && atom->getArity() == 1) continue;
		if (atom->getPredicate().getString() == std::string("leafnode") && atom->getArity() == 2) continue;
		if (atom->getPredicate().getString() == std::string("conditionaledge") && atom->getArity() == 5) continue;
		if (atom->getPredicate().getString() == std::string("elseedge") && atom->getArity() == 2) continue;

		throw DecisionDiagram::InvalidDecisionDiagram(std::string("Error: An illegal atom was found: ") + atom->getPredicate().getString() + std::string("/") + StringHelper::toString((int)atom->getArity()) + std::string("Input programs must only contain atoms over root/1, innernode/1, leafnode/2, conditionaledge/5 and elseedge/2."));
	}

	// create a diagram (can throw exceptions)
	DecisionDiagram* dd = new DecisionDiagram(atoms);
	return dd;
}

void HexFormat::write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram){
	// create an answer-set
	AtomSet as = dd->toAnswerSet();
	// print it as dlv program
	DLVPrintVisitor pv(std::cout);
	pv.visit(&as);
}
