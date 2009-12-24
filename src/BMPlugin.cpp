
//
// this include is necessary
//
#include "dlvhex/PluginInterface.h"

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>
#include <stdlib.h>

#include <Parser.h>
#include <CodeGenerator.h>

namespace dlvhex {
	namespace bm {

		class BeliefMergingParserDriver : public PluginConverter
		{
		public:
			BeliefMergingParserDriver()
			{
			}

			virtual void
			convert(std::istream& i, std::ostream& o)
			{
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
				// - Call the belief merging compiler
				// - On errors throw a PluginError
				// - Otherwise write the compiler output into o; dlvhex will execute it
			}
		};

		//
		// A plugin must derive from PluginInterface
		//
		class BMPlugin : public PluginInterface
		{
		private:
			PluginConverter *converter;

		public:
			BMPlugin() : converter(NULL){
			}

			virtual ~BMPlugin(){
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
					out	<< "BM-plugin:" << std::endl << std::endl
						<< " --beliefmerging  This option will cause BM-plugin to parse the complete" << std::endl
						<< "                  input file as belief merging program. For details see" << std::endl
						<< "                  the documentation of BM-plugin" << std::endl << std::endl;
				}
			}

		};


		//
		// now instantiate the plugin
		//
		BMPlugin theBMPlugin;

	} // namespace bm
} // namespace dlvhex

//
// and let it be loaded by dlvhex!
//
extern "C"
dlvhex::bm::BMPlugin*
PLUGINIMPORTFUNCTION()
{

  dlvhex::bm::theBMPlugin.setPluginName("dlvhex-BMPlugin");
  dlvhex::bm::theBMPlugin.setVersion(	0,
					0,
					1);
  return &dlvhex::bm::theBMPlugin;
}


