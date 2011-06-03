#include <OpASP.h>

#include <dlvhex/AggregateAtom.h>
#include <dlvhex/DLVProcess.h>
#include <dlvhex/ASPSolverManager.h>
#include <dlvhex/ASPSolver.h>
#include <dlvhex/Registry.h>
#include <dlvhex/HexParserDriver.h>

#include <dlvhex/PrintVisitor.h>
#include <dlvhex/DLVProcess.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>

using namespace dlvhex;
using namespace dlvhex::merging;
using namespace dlvhex::dd;
using namespace dlvhex::dd::plugin;

std::string OpASP::getName(){
	return "asp";
}

std::string OpASP::getInfo(){
	std::stringstream ss;
	ss <<	"     asp" << std::endl <<
		"     ---" << std::endl << std::endl <<
		 "    &operator[\"asp\", A, K](A)" << std::endl <<
		 "         A(H1), ..., A(Hn)    ... Handles to n sets of decision diagrams" << std::endl <<
		 "         K(program, c)        ... c = arbitrary ASP code" << std::endl <<
		 "         K(file, f)           ... f = the name of a file with ASP code" << std::endl <<
		 "         K(maxint, i)       . i = maximum integer value to be passed to the ASP reasoner" << std::endl <<
		 "    The operator will add the input decision diagrams as facts to the user defined program before executing it," << std::endl <<
		 "    where the diagrams are encoded as:" << std::endl <<
		 "         rootIn(I,N)" << std::endl <<
		 "         innernodeIn(I,N)" << std::endl <<
		 "         leafnodeIn(I,N,C)" << std::endl <<
		 "         conditionaledgeIn(I,N1,N2,O1,C,O2)" << std::endl <<
		 "         elseedgeIn(I,N1,N2)" << std::endl <<
		 "    where I is a running index denoting the number of the input decision diagram." << std::endl <<
		 "    " << std::endl <<
		 "    Additionally the fact ddcountIn(C) is added, which encodes the number of input diagrams." << std::endl <<
		 "    " << std::endl <<
		 "    The user-defined program is expected to produce zero to arbitrary many output decision diagrams (exactly one per answer set)," << std::endl <<
		 "    encoded as:" << std::endl <<
		 "         root(N)" << std::endl <<
		 "         innernode(N)" << std::endl <<
		 "         leafnode(N,C)" << std::endl <<
		 "         conditionaledge(N1,N2,O1,C,O2)" << std::endl <<
		 "         elseedge(N1,N2)" << std::endl;
	return ss.str();
}

std::set<std::string> OpASP::getRecognizedParameters(){
	std::set<std::string> list;
	list.insert("program");
	list.insert("file");
	list.insert("maxint");
	return list;
}

/**
 * Parses the following parameters:
 * 	-) program: "some ASP program"
 * 	-) file: "some filename"
 * 	-) maxint: maximum integer value to be passed to the reasoner
 * \param parameters Reference to the set of parameters to write to
 * \param maxint Reference to the integer where the maximum int value shall be written to
 * \param program Reference to the program where constraints shall be appended
 * \param facts Reference to the facts part of the program where constraints shall be appended
 */
void OpASP::parseParameters(OperatorArguments& parameters, int& maxint, dlvhex::Program& program, dlvhex::AtomSet& facts){

	bool penalizeSet = false;

	// process additional parameters
	for (OperatorArguments::iterator argIt = parameters.begin(); argIt != parameters.end(); argIt++){

		// add program code
		if (argIt->first == std::string("program")){
			std::string programcode = argIt->second;
			// parse it
			try{
				HexParserDriver hpd;
				std::stringstream ss(programcode);
				hpd.parse(ss, program, facts);
			}catch(SyntaxError){
				throw IOperator::OperatorException(std::string("Could not parse program due to a syntax error: \"") + argIt->second + std::string("\""));
			}

		// add program code from a file
		}else if (argIt->first == std::string("file")){
			try{
				// read input
				std::ifstream inp;
				inp.open(argIt->second.c_str());
				std::string s;
				std::stringstream programcode;
				while (inp.good() && std::getline(inp, s)){
					programcode << s << std::endl;
				}

				// parse it
				std::stringstream ss(programcode.str());
				HexParserDriver hpd;
				hpd.parse(ss, program, facts);
			}catch(SyntaxError){
				throw IOperator::OperatorException(std::string("Could not parse program file due to a syntax error: \"") + argIt->second + std::string("\""));
			}

		// extract maxint
		}else if (argIt->first == std::string("maxint")){
			maxint = atoi(argIt->second.c_str());
			if (maxint <= 0) throw IOperator::OperatorException(std::string("maxint must be a positive integer. \"") + argIt->second + std::string("\" was passed"));
		}
	}
}

HexAnswer OpASP::apply(bool debug, int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters) throw (OperatorException){

	// create a subprogram that computes the user defined ASP code
	dlvhex::Program program;
	dlvhex::AtomSet facts;

	int maxint = 0;
	parseParameters(parameters, maxint, program, facts);

	// ---------- start building the program ----------

	// add the ASP encoded decision diagrams as facts
	std::vector<HexAnswer> sources;

	int ddNr = 0;
	for (int answer = 0; answer < arity; answer++){
		for (int answerset = 0; answerset < arguments[answer]->size(); answerset++){
			// add this decision diagram as facts to the program
			// Note: We do not use the usual encoding (root, innernode, leafnode, conditionaledge, elseedge) here,
			//	 but rather add an additional parameter (index) at the 0-th position. This allows for encoding
			//	 multiple diagrams within one set of facts.
			//	 Furthermore, the fact "diagramNumber(N)" will encode the number of decision diagrams.
			DecisionDiagram dd((*arguments[answer])[answerset]);
			facts.insert(dd.toAnswerSet(true, ddNr++));
		}
	}
	if (ddNr > maxint) maxint = ddNr;

	// add information on the number of decision diagrams
	Tuple ddNrArgs;
	ddNrArgs.push_back(Term(ddNr));
	AtomPtr ddNrAtom = AtomPtr(new Atom("ddcountIn"));
	ddNrAtom->setArguments(ddNrArgs);
	facts.insert(ddNrAtom);

	// build the resulting program and execute it
	try{
		ASPSolverManager& solver = ASPSolverManager::Instance();
		typedef ASPSolverManager::SoftwareConfiguration<ASPSolver::DLVSoftware> DLVConfiguration;
		DLVConfiguration dlv;
		dlv.options.includeFacts = true;
		std::stringstream maxint_str;
		maxint_str << "-N=" << maxint;
		dlv.options.arguments.push_back(maxint_str.str());
		dlv.options.arguments.push_back(std::string("-filter=root,innernode,leafnode,conditionaledge,elseedge"));
		std::vector<AtomSet> result;
		solver.solve(dlv, program, facts, result);

		// the result contains now the filtered or processed decision diagrams
		return result;
	}catch(...){
		std::stringstream ss;
		if (debug){
			ss << ": " << std::endl;
			DLVPrintVisitor pv(ss);
			pv.PrintVisitor::visit(&program);
			pv.PrintVisitor::visit(&facts);
		}
		throw OperatorException(std::string("Error while building and executing program") + ss.str());
	}
}
