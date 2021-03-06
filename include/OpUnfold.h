#ifndef __OPUNFOLD_H_
#define __OPUNFOLD_H_

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
			 * This class implements the unfolding operator. It assumes each answer to represent a general decision diagram and translates it into a tree-like one .
			 * Usage:
			 * <pre>
			 * &operator["unfold", DD, K](A)
			 *	DD	... predicate with index 0 and handle to exactly 1 answer containing arbitrary many decision diagram
			 *	A	... answer to the operator result
			 * </pre>
			 */
			class OpUnfold : public IOperator{
			private:
				DecisionDiagram unfold(DecisionDiagram::Node* root, DecisionDiagram& ddin);
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
