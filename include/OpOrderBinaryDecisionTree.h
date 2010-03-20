#ifndef __OPORDERDECISIONTREE_H_
#define __OPORDERDECISIONTREE_H_

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
			 * This class implements the tree ordering operator. It assumes each answer to represent a binary decision tree and translates it into an ordered one.
			 * Usage:
			 * &operator["orderbinarydecisiontree", DD, K](A)
			 *	DD	... predicate with index 0 and handle to exactly 1 answer containing arbitrary many decision diagram
			 *	A	... answer to the operator result (answer containing ordered binary decision diagrams)
			 */
			class OpOrderBinaryDecisionTree : public IOperator{
			private:
				std::string getCompareAttribute(DecisionDiagram::Node* node);
				DecisionDiagram::Node* sink(DecisionDiagram& dd, DecisionDiagram::Node* root);
				DecisionDiagram::Node* order(DecisionDiagram& dd, DecisionDiagram::Node* root);
			public:
				virtual std::string getName();
				virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
			};
		}
	}
}

#endif
