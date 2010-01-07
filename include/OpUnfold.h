#ifndef __OPUNFOLD_H_
#define __OPUNFOLD_H_

#include "dlvhex-mergingplugin/IOperator.h"
#include <DecisionDiagram.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging;

namespace dlvhex{
	namespace dd{

		/**
		 * This class implements the unfolding operator. It assumes each answer to represent a general decision diagram and translates it into a tree-like one .
		 * Usage:
		 * &operator["unfold", DD](A)
		 *	DD	... handle to an answer containing decision diagrams
		 *	A	... answer to the operator result
		 */
		class OpUnfold : public IOperator{
		private:
			DecisionDiagram unfold(DecisionDiagram::Node* root, DecisionDiagram& ddin);
		public:
			virtual std::string getName();
			virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters);
		};
	}
}

#endif
