#ifndef __OPTOBINARYDECISIONTREE_H_
#define __OPTOBINARYDECISIONTREE_H_

#include "dlvhex-mergingplugin/IOperator.h"
#include <DecisionDiagram.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging::plugin;
using namespace dlvhex::dd::util;

namespace dlvhex{
	namespace dd{
		namespace plugin{
			/**
			 * \brief
			 * This class implements the to-binary operator. It assumes each answer to represent a general decision tree and translates it into a binary one.
			 * Usage:
			 * <pre>
			 * &operator["tobinarydecisiontree", DD](A)
			 *	DD	... handle to an answer containing arbitrary many general decision trees
			 *	A	... answer to the operator result (answer containing binary decision diagrams)
			 * </pre>
			 */
			class OpToBinaryDecisionTree : public IOperator{
			private:
				void toBinary(DecisionDiagram& dd, DecisionDiagram::Node* root);
			public:
				virtual std::string getName();
				virtual std::string getInfo();
				virtual std::set<std::string> getRecognizedParameters();
				virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
			};
		}
	}
}

#endif
