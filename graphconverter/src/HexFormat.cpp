#include <HexFormat.h>

#include <dlvhex/HexParserDriver.h>
#include <dlvhex/DLVresultParserDriver.h>
#include <dlvhex/PrintVisitor.h>
#include <dlvhex/Program.h>

#include <iostream>
#include <string>
#include <sstream>

std::string HexFormat::getName(){
	return "hexprogram";
}

std::string HexFormat::getNameAbbr(){
	return "hex";
}

DecisionDiagram* HexFormat::read(){
	// parse the hex program (a set of facts)
	HexParserDriver hexparser;
	Program prog;
	AtomSet atoms;
	hexparser.parse(std::cin, prog, atoms);

	// check if the program consists of facts only
	for (Program::iterator rule = prog.begin(); rule != prog.end(); ++rule){
		std::cerr << "Error: A rule was found. The input program must not contain rules, but facts only." << std::endl;
		return NULL;
	}

	// check if the program contains only legal predicates with legal arities
	for (AtomSet::const_iterator atom = atoms.begin(); atom != atoms.end(); ++atom){
		if (atom->getPredicate().getString() == std::string("root") && atom->getArity() == 1) continue;
		if (atom->getPredicate().getString() == std::string("innernode") && atom->getArity() == 1) continue;
		if (atom->getPredicate().getString() == std::string("leafnode") && atom->getArity() == 2) continue;
		if (atom->getPredicate().getString() == std::string("conditionaledge") && atom->getArity() == 5) continue;
		if (atom->getPredicate().getString() == std::string("elseedge") && atom->getArity() == 2) continue;

		std::cerr << "Error: An illegal atom was found: " << atom->getPredicate().getString() << "/" << atom->getArity() << "." << std::endl << std::endl << "Input programs must only contain atoms over root/1, innernode/1, leafnode/2, conditionaledge/5 and elseedge/2." << std::endl;
		return NULL;
	}

	// create a diagram
	DecisionDiagram* dd = new dlvhex::dd::DecisionDiagram(atoms);
	return dd;
}

bool HexFormat::write(DecisionDiagram* dd){
	// create an answer-set
	AtomSet as = dd->toAnswerSet();
	// print it as dlv program
	DLVPrintVisitor pv(std::cout);
	pv.visit(&as);
	return true;
}
