#ifndef __OPMAJORITYVOTING_H_
#define __OPMAJORITYVOTING_H_

#include "dlvhex-mergingplugin/IOperator.h"
#include <DecisionDiagram.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging;

namespace dlvhex{
	namespace dd{

		/**
		 * This class implements the majority voting operator. It assumes that 2 answers are passed to the operator (binary operator) with one decision diagram each.
		 * The result will be another decision diagram which delivers the same answer as the input, iff the two diagrams agree upon the classification of an element. Otherwise
		 * the classification will be "unknown".
		 * Usage:
		 * &operator["majorityvoting", DD](A)
		 *	DD	... predicate with handles to exactly 2 answers containing one decision diagram each
		 *	A	... answer to the operator result
		 */
		class OpMajorityVoting : public IOperator{
		protected:
			typedef std::map<std::string, int> Votings;
			void insert(DecisionDiagram& input, DecisionDiagram& output, std::map<DecisionDiagram::LeafNode*, Votings>& votings);
		public:
			virtual std::string getName();
			virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
		};
	}
}

#endif
