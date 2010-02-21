
//
// this include is necessary
//
#include "dlvhex/PluginInterface.h"

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>

//#include <Parser.h>
//#include <CodeGenerator.h>

#include "dlvhex-mergingplugin/IOperator.h"
#include <OpUnfold.h>
#include <OpMajorityVoting.h>
#include <OpToBinaryDecisionTree.h>
#include <OpUserPreferences.h>
#include <OpOrderBinaryDecisionTree.h>
#include <OpAvg.h>
#include <OpSimplify.h>

namespace dlvhex {
	namespace dd {

		class BeliefMergingParserDriver : public PluginConverter
		{
		public:
			BeliefMergingParserDriver()
			{
			}

			virtual void
			convert(std::istream& i, std::ostream& o)
			{
/*
	THIS CODE SECTION WORKS
				// - Call the belief merging compiler
				RPProcess rpp;
				rpp.spawn();

				std::istream& pi = rpp.getInput();
				std::ostream& po = rpp.getOutput();
				std::string line;

				// Send input to rpcompiler
				while(getline(i, line)) {
					po << line << std::endl;
				}
				rpp.endoffile();

				// Read output from rpcomopiler
				std::stringstream rpoutput;
				while(getline(pi, line)) {
					rpoutput << line << std::endl;
				}

				// On errors throw a PluginError
				int errcode;
				if((errcode = rpp.close()) != 0){
					// Read error message from rpcompiler
					std::stringstream errmsg;
					errmsg << std::string("rpcompiler returned error code ") << errcode << ": " << rpoutput.str();
					throw PluginError(errmsg.str());
				}else{
					// Otherwise write the compiler output into o; dlvhex will execute it
					o << rpoutput.str();
					// Workaround (dlvhex crashes on empty input)
					o << "%" << std::endl;
				}

				return;
*/

/*
	THIS ONE NOT
				std::string line;
				std::stringstream buffer;
				// Read whole program into a buffer (belief merging program)
				while(getline(i,line)) {
					buffer << line << std::endl;
				}

				// Try to parse the input
				Parser parserinst(buffer.str());
				parserinst.parse();

				// Generate output code
				if (parserinst.succeeded()){
					// Retrieve parse tree
					ParseTreeNode* parseTree = parserinst.getParseTree();

					// GenerateCode
					// On success, write the code to std::cout and "errors" (in this case warnings) to std::cerr
					CodeGenerator cginst(parseTree);
					cginst.generateCode(o, std::cerr);

					if (!cginst.succeeded()){
						// Code generation failed
						std::stringstream errormsg;
						errormsg << "Code generation finished with errors:" << std::endl;
						errormsg << "   " << cginst.getErrorCount() << " error" << (cginst.getErrorCount() == 0 || cginst.getErrorCount() > 1 ? "s" : "") << ", " << cginst.getWarningCount() << " warning" << (cginst.getWarningCount() == 0 || cginst.getWarningCount() > 1 ? "s" : "") << std::endl;
						delete parseTree;
						throw PluginError(errormsg.str());
					}

					delete parseTree;
				}else{
					// Parsing failed
					std::stringstream errormsg;
					errormsg << "Parsing finished with errors:" << std::endl;
					errormsg << "   " << parserinst.getErrorCount() << " error" << (parserinst.getErrorCount() == 0 || parserinst.getErrorCount() > 1 ? "s" : "") << ", " << parserinst.getWarningCount() << " warning" << (parserinst.getWarningCount() == 0 || parserinst.getWarningCount() > 1 ? "s" : "") << std::endl;
					throw PluginError(errormsg.str());
				}

				// Succeeded. Code was written to o.


				// TODO
*/
			}
		};

		//
		// A plugin must derive from PluginInterface
		//
		class DecisionDiagramPlugin : public PluginInterface
		{
		private:
			PluginConverter *converter;

		public:
			DecisionDiagramPlugin() : converter(NULL){
			}

			virtual ~DecisionDiagramPlugin(){
				if (converter) delete converter;
			}

			virtual PluginConverter*
			createConverter()
			{
				return converter;
			}


			//
			// register all atoms of this plugin:
			//
			virtual void
			getAtoms(AtomFunctionMap& a)
			{
			}

			virtual void
			setOptions(bool doHelp, std::vector<std::string>& argv, std::ostream& out)
			{
				if (!doHelp){
					for (std::vector<std::string>::iterator it = argv.begin(); it != argv.end(); it++){
						if (*it == std::string("--beliefmerging")){
							converter = new BeliefMergingParserDriver();
							argv.erase(it);
							break;	// iterator is invalid now
						}
					}
				}else{
					out	<< "DD-plugin:" << std::endl << std::endl
						<< "----------" << std::endl
						<< " Arguments:" << std::endl
						<< " --beliefmerging  This option will cause DD-plugin to parse the complete" << std::endl
						<< "                  input file as belief merging program. Internally, the" << std::endl
						<< "                  rpcompiler will be called. For details see the" << std::endl
						<< "                  documentation of BM-plugin" << std::endl << std::endl;
				}
			}

		};


		//
		// now instantiate the plugin
		//
		DecisionDiagramPlugin theDDPlugin;

	} // namespace bm
} // namespace dlvhex

using namespace dlvhex::dd;

OpUnfold _unfold;
OpMajorityVoting _majorityvoting;
OpToBinaryDecisionTree _tobinarydecisiontree;
OpUserPreferences _userpreferences;
OpOrderBinaryDecisionTree _orderbinarydecisiontree;
OpAvg _avg;
OpSimplify _simplify;

extern "C"
std::vector<dlvhex::merging::IOperator*>
OPERATORIMPORTFUNCTION()
{
	std::vector<dlvhex::merging::IOperator*> operators;
	operators.push_back(&_unfold);
	operators.push_back(&_majorityvoting);
	operators.push_back(&_tobinarydecisiontree);
	operators.push_back(&_userpreferences);
	operators.push_back(&_orderbinarydecisiontree);
	operators.push_back(&_avg);
	operators.push_back(&_simplify);
	return operators;
}

//
// and let it be loaded by dlvhex!
//
extern "C"
dlvhex::dd::DecisionDiagramPlugin*
PLUGINIMPORTFUNCTION()
{

  dlvhex::dd::theDDPlugin.setPluginName("dlvhex-DecisionDiagramPlugin");
  dlvhex::dd::theDDPlugin.setVersion(	0,
					0,
					1);
  return &dlvhex::dd::theDDPlugin;
}


