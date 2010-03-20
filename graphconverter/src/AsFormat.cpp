#include <AsFormat.h>
#include <StringHelper.h>

#include <dlvhex/HexParserDriver.h>
#include <dlvhex/DLVresultParserDriver.h>
#include <dlvhex/PrintVisitor.h>
#include <dlvhex/Program.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
using namespace dlvhex::dd::tools::graphconverter;

std::string AsFormat::getName(){
	return "answerset";
}

std::string AsFormat::getNameAbbr(){
	return "as";
}

DecisionDiagram* AsFormat::read() throw (DecisionDiagram::InvalidDecisionDiagram){
	// parse the answerset
	DLVresultParserDriver answersetParser;
	vector<AtomSet> answersets;
	answersetParser.parse(std::cin, answersets);
	if (answersets.size() != 1){
		throw DecisionDiagram::InvalidDecisionDiagram(std::string("Error: ") + StringHelper::toString((int)answersets.size()) + std::string("answer-sets were passed, but exactly one is expected"));
	}else{
		// translate atomset into the internal data structure (can throw exceptions)
		DecisionDiagram* dd = new DecisionDiagram(answersets[0]);
		return dd;
	}
}

void AsFormat::write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram){
	// create an answer-set
	AtomSet as = dd->toAnswerSet();
	// print it atom by atom with separator ','
	DLVPrintVisitor pv(std::cout);
	pv.PrintVisitor::visit(&as);
}
