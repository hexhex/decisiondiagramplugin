#ifndef __OPMAJORITYVOTING_H_
#define __OPMAJORITYVOTING_H_

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
			 * This class implements the majority voting operator. It assumes that 2 answers are passed to the operator (binary operator) with one decision diagram each.
			 * The result will be another decision diagram which delivers the same answer as the input, iff the two diagrams agree upon the classification of an element. Otherwise
			 * the classification will be "unknown".
			 * Usage:
			 * <pre>
			 * &operator["majorityvoting", DD, K](A)
			 *	DD	... predicate with indices and handles to exactly 2 answers containing one decision diagram each
			 *	A	... answer to the operator result
			 * </pre>
			 */
			class OpMajorityVoting : public IOperator{
			protected:
				typedef std::map<std::string, int> Votings;
				/**
				 * Stores a map containing labels (strings) paired with their voting counts (integers).
				 */
				struct Votes : public DecisionDiagram::LeafNode::Data{
					Votings v;
				};
				virtual void insert(DecisionDiagram& input, DecisionDiagram& output);
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
