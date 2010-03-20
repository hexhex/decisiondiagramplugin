#ifndef __OPDISTRIBUTIONMAPVOTING_H_
#define __OPDISTRIBUTIONMAPVOTING_H_

#include "dlvhex-mergingplugin/IOperator.h"
#include <OpMajorityVoting.h>
#include <DecisionDiagram.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging::plugin;
using namespace dlvhex::dd::util;

namespace dlvhex{
	namespace dd{
		namespace plugin{
			/**
			 * \brief
			 * This class implements an operator that merged diagrams according to the distribution maps in their leaf nodes.
			 * It assumes that 2 answers are passed to the operator (binary operator) with one decision diagram each. The leaf nodes do not only need to contain the classification, but also
			 * the distribution of training examples over the classes which ended in this leaf. For instance, if 4 training examples belong to class1 and 2 to class2, the classification is
			 * "class1" and the distribution map is "class1:4,class2:2".
			 * The syntax is as follows:
			 * 	"classification {c1:n1,c2:n2,...,cn:nm}"
			 * The result will be another decision diagram which combines diagrams by inserting the second into all leaf nodes of the first one, and recomputing the classification according
			 * to the (combined) distribution map.
			 * Usage:
			 * &operator["majorityvoting", DD, K](A)
			 *	DD	... predicate with indices 0-1 and handles to exactly 2 answers containing one decision diagram each
			 *	A	... answer to the operator result
			 */
			class OpDistributionMapVoting : public OpMajorityVoting{
			protected:
				virtual void insert(DecisionDiagram& input, DecisionDiagram& output);
			public:
				virtual std::string getName();
				virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
			};
		}
	}
}

#endif
