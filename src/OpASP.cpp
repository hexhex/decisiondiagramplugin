#include <OpASP.h>

#include <dlvhex/AggregateAtom.h>
#include <dlvhex/DLVProcess.h>
#include <dlvhex/ASPSolverManager.h>
#include <dlvhex/ASPSolver.h>
#include <dlvhex/Registry.h>
#include <dlvhex/HexParserDriver.h>

#include <dlvhex/PrintVisitor.h>
#include <DLVHexProcess.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>

using namespace dlvhex;
using namespace dlvhex::merging;
using namespace dlvhex::merging::plugin;

std::string OpASP::getName(){
	return "asp";
}

std::string OpASP::getInfo(){
	std::stringstream ss;
	ss <<	"     asp" << std::endl <<
		"     ---" << std::endl << std::endl <<
		 " &operator[\"asp\", A, K](A)" << std::endl <<
		 "	A(H1), ..., A(Hn)	... Handles to n decision diagrams" << std::endl <<
		 "	K(program, c)		... c = arbitrary ASP code" << std::endl <<
		 "	K(file, f)		... f = the name of a file with ASP code" << std::endl;
		 "	K(maxint, m)		... m = maximum integer value to be passed to the reasoner" << std::endl;
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
 * \param arity The number of answer arguments
 * \param optAtom The unique name of the atom upon which optimization occurs
 * \param costSum The unique name of the atom where sourcewise cost aggregation occurs
 * \param weights Reference to the vector where the weights shall be written to
 * \param usedAtoms The list of atoms occurring in any source program
 * \param maxint Reference to the integer where the maximum int value shall be written to
 * \param ignoredPredicates Reference to the set where the ignored predicates shall be written to
 * \param program Reference to the program where constraints shall be appended
 * \param facts Reference to the facts part of the program where constraints shall be appended
 * \param aggregation Reference to the string where the aggregation function shall be written to
 */
void OpASP::parseParameters(int& maxint, dlvhex::Program& program, dlvhex::AtomSet& facts){

	bool penalizeSet = false;

	// process additional parameters
	for (OperatorArguments::iterator argIt = parameters.begin(); argIt != parameters.end(); argIt++){

		// add program code
		if (argIt->first == std::string("program")){
			std::string program = argIt->second;
			// parse it
			try{
				HexParserDriver hpd;
				std::stringstream ss(program);
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

	// Make dlvhex believe we work in firstorder mode
	// This is a workaround due to a nasty assertion in dlvhex: otherwise we can not instanciate AggregateAtom
	unsigned noPred = Globals::Instance()->getOption("NoPredicate");
	Globals::Instance()->setOption("NoPredicate", 0);

	// create a subprogram that computes the user defined ASP code
	dlvhex::Program program;
	dlvhex::AtomSet facts;

	parseParameters(program, facts);



	// ---------- start building the program ----------

	// add the ASP encoded decision diagrams as facts
	std::vector<HexAnswer> sources;

	// all sources
	int sourceIndex = 0;
	for (std::vector<HexAnswer*>::iterator argIt = arguments.begin(); argIt != arguments.end(); argIt++, sourceIndex++){

		// all decision diagrams from this source
		for (AtomSet::iterator asIt = argIt->begin(); asIt != arguments.end(); asIt++){
			// interpret as decision diagram
			DecisionDiagram dd(*asIt);

			// add this decision diagram as facts to the program
			// Note: We do not use the usual encoding (root, innernode, leafnode, conditionaledge, elseedge) here,
			//	 but rather add an additional parameter (index) at the 0-th position. This allows for encoding
			//	 multiple diagrams within one set of facts.
			//	 Furthermore, the fact "diagramNumber(N)" will encode the number of decision diagrams.
			facts.add(dd.toAnswerSet(sourceIndex));
		}
	}
	// add information on the number of decision diagrams
	Tuple args;
	args.push_back(Term(sourceIndex));
	facts.add(new Atom(std::string("diagramNumber"), args));


	// build the resulting program and execute it
	try{
		ASPSolverManager& solver = ASPSolverManager::Instance();
		typedef ASPSolverManager::SoftwareConfiguration<ASPSolver::DLVSoftware> DLVConfiguration;
		DLVConfiguration dlv;
		std::stringstream maxint_str;
		maxint_str << "-N=" << maxint;
		dlv.options.arguments.push_back(maxint_str.str());
		dlv.options.arguments.push_back(std::string("-filter=root,innernode,leafnode,conditionaledge,elseedge"));
		std::vector<AtomSet> result;
		solver.solve(dlv, program, facts, result);

		// the result contains now the filtered or processed decision diagrams

		// restore original setting of NoPredicate
    // @todo this "NoPredicate" is no longer in use, higher order is configured in *Software::Options
		Globals::Instance()->setOption("NoPredicte", noPred);

		return result;
	}catch(...){
		// restore original setting of NoPredicate
		Globals::Instance()->setOption("NoPredicate", noPred);

		std::stringstream ss;
		if (debug){
			ss << ": " << std::endl;
			DLVPrintVisitor pv(ss);
			pv.PrintVisitor::visit(&program);
			pv.PrintVisitor::visit(&facts);
		}
		throw OperatorException("Error while building and executing program: " << std::endl << ss.str());
	}
}
