#ifndef __OPSIMPLIFY_H_
#define __OPSIMPLIFY_H_

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
			 * This class implements the simplification operator.
			 * Usage:
			 * &operator["simplify", DD, K](A)
			 *	DD	... handle to an answer containing arbitrary many binary decision diagrams
			 *	A	... answer to the operator result (answer containing simplified decision diagrams)
			 */
			class OpSimplify : public IOperator{
			private:
				DecisionDiagram::Node* reduceSubgraph(DecisionDiagram& dd, DecisionDiagram::Node* n);
				void removeConnectiveComponent(DecisionDiagram& dd, DecisionDiagram::Node* n);
				DecisionDiagram simplify(DecisionDiagram dd);
			public:
				virtual std::string getName();
				virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
			};
		}
	}
}

#endif
