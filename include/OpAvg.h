#ifndef __OPAVG_H_
#define __OPAVG_H_

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
			 * This class implements the average operator. It assumes that 2 answers are passed to the operator (binary operator) with one ordered binary diagram tree each.
			 * The result will be another ordered binary diagram tree where all constants in range queries are averaged if the input decision trees differ.
			 * Usage:
			 * <pre>
			 * &operator["average", DD, K](A)
			 *	DD	... predicate with handles to exactly 2 answers containing one ordered binary decision tree each
			 *	A	... answer to the operator result
			 * </pre>
			 */
			class OpAvg : public IOperator{
				DecisionDiagram::Condition getCondition(DecisionDiagram::Node* node);
				DecisionDiagram::Node* averageLeafs(DecisionDiagram& result, DecisionDiagram* dd1, DecisionDiagram::LeafNode* leaf1, DecisionDiagram* dd2, DecisionDiagram::LeafNode* leaf2);
				DecisionDiagram::Node* averageInner(DecisionDiagram& result, DecisionDiagram* dd1, DecisionDiagram::Node* inner1, DecisionDiagram* dd2, DecisionDiagram::Node* inner2);
				DecisionDiagram::Node* averageInnerLeaf(DecisionDiagram& result, DecisionDiagram* innerdiag, DecisionDiagram::Node* inner, DecisionDiagram* leafdiag, DecisionDiagram::LeafNode* leaf);
				DecisionDiagram::Node* average(DecisionDiagram& result, DecisionDiagram* dd1, DecisionDiagram::Node* n1, DecisionDiagram* dd2, DecisionDiagram::Node* n2);
			public:
				virtual std::string getName();
				virtual std::string getInfo();
				virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
			};
		}
	}
}

#endif
