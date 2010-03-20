
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

#include "dlvhex-mergingplugin/IOperator.h"
#include <OpUnfold.h>
#include <OpMajorityVoting.h>
#include <OpDistributionMapVoting.h>
#include <OpToBinaryDecisionTree.h>
#include <OpUserPreferences.h>
#include <OpOrderBinaryDecisionTree.h>
#include <OpAvg.h>
#include <OpSimplify.h>

namespace dlvhex {
	namespace dd {
		namespace plugin{
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
					if (doHelp){
						out	<< "DD-plugin:" << std::endl << std::endl
							<< "----------" << std::endl
							<< " The DD-plugin is a pure operator library for mergingplugin." << std::endl
							<< " It does not contain any external atoms." << std::endl << std::endl;
					}
				}

			};


			//
			// now instantiate the plugin
			//
			DecisionDiagramPlugin theDDPlugin;
		} // namespace plugin
	} // namespace dd
} // namespace dlvhex

using namespace dlvhex::dd::plugin;

OpUnfold _unfold;
OpMajorityVoting _majorityvoting;
OpDistributionMapVoting _distributionmapvoting;
OpToBinaryDecisionTree _tobinarydecisiontree;
OpUserPreferences _userpreferences;
OpOrderBinaryDecisionTree _orderbinarydecisiontree;
OpAvg _avg;
OpSimplify _simplify;

extern "C"
std::vector<dlvhex::merging::plugin::IOperator*>
OPERATORIMPORTFUNCTION()
{
	std::vector<dlvhex::merging::plugin::IOperator*> operators;
	operators.push_back(&_unfold);
	operators.push_back(&_majorityvoting);
	operators.push_back(&_distributionmapvoting);
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
dlvhex::dd::plugin::DecisionDiagramPlugin*
PLUGINIMPORTFUNCTION()
{

  dlvhex::dd::plugin::theDDPlugin.setPluginName("dlvhex-DecisionDiagramPlugin");
  dlvhex::dd::plugin::theDDPlugin.setVersion(	0,
						9,
						0);
  return &dlvhex::dd::plugin::theDDPlugin;
}


