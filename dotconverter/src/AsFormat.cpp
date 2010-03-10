#include <AsFormat.h>

#include <dlvhex/HexParserDriver.h>
#include <dlvhex/DLVresultParserDriver.h>
#include <dlvhex/PrintVisitor.h>
#include <dlvhex/Program.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

std::string AsFormat::getName(){
	return "answerset";
}

std::string AsFormat::getNameAbbr(){
	return "as";
}

DecisionDiagram* AsFormat::read(){
	// parse the answerset
	DLVresultParserDriver answersetParser;
	vector<AtomSet> answersets;
	answersetParser.parse(std::cin, answersets);
	if (answersets.size() != 1){
		std::cerr << answersets.size() << "answer-sets were passed, but exactly one is expected" << std::endl;
		return NULL;
	}else{
		// translate atomset into the internal data structure
		DecisionDiagram* dd = new DecisionDiagram(answersets[0]);
		return dd;
	}
}

bool AsFormat::write(DecisionDiagram* dd){
	// create an answer-set
	AtomSet as = dd->toAnswerSet();
	// print it atom by atom with separator ','
	DLVPrintVisitor pv(std::cout);
	pv.PrintVisitor::visit(&as);
	return true;
}
