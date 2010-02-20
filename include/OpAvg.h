#ifndef __OPAVG_H_
#define __OPAVG_H_

#include "dlvhex-mergingplugin/IOperator.h"
#include <DecisionDiagram.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging;

namespace dlvhex{
	namespace dd{

		/**
		 * This class implements the average operator. It assumes that 2 answers are passed to the operator (binary operator) with one ordered binary diagram tree each.
		 * The result will be another ordered binary diagram tree where all constants in range queries are averaged if the input decision trees differ.
		 * Usage:
		 * &operator["average", DD](A)
		 *	DD	... predicate with handles to exactly 2 answers containing one ordered binary decision tree each
		 *	A	... answer to the operator result
		 */
		class OpAvg : public IOperator{
			std::string unquote(std::string str);
			DecisionDiagram::Condition getCondition(DecisionDiagram::Node* node);
			DecisionDiagram::Node* average(DecisionDiagram* result, DecisionDiagram* dd1, DecisionDiagram::Node* n1, DecisionDiagram* dd2, DecisionDiagram::Node* n2);
		public:
			virtual std::string getName();
			virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
		};
	}
}

#endif
