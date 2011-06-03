#ifndef __OPASP_H_
#define __OPASP_H_

#include "dlvhex-mergingplugin/IOperator.h"

#include <DecisionDiagram.h>
#include <dlvhex/Program.h>
#include <dlvhex/AtomSet.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging::plugin;
using namespace dlvhex::dd::util;

namespace dlvhex{
	namespace dd{
		namespace plugin{
			/**
			 * This class implements an operator which allows for calling ASP programs which reason over decision diagrams.
			 * Usage:
			 * &operator["asp", A, K](A)
			 *	A(H1), ..., A(Hn)	... Handles to n sets of decision diagrams
			 *	K(program, c)		... c = arbitrary ASP code
			 *	K(file, f)		... f = the name of a file with ASP code
			 *	K(maxint, i)		... i = maximum integer value to be passed to the ASP reasoner
			 * The operator will add the input decision diagrams as facts to the user defined program before executing it,
			 * where the diagrams are encoded as:
			 *      rootIn(I,N)
			 *      innernodeIn(I,N)
			 *      leafnodeIn(I,N,C)
			 *      conditionaledgeIn(I,N1,N2,O1,C,O2)
			 *      elseedgeIn(I,N1,N2)
			 * where I is a running index denoting the number of the input decision diagram.
			 * Additionally the fact ddcountIn(C) is added, which encodes the number of input diagrams.
			 * The user-defined program is expected to produce zero to arbitrary many output decision diagrams (exactly one per answer set),
			 * encoded as:
			 *      root(N)
			 *      innernode(N)
			 *      leafnode(N,C)
			 *      conditionaledge(N1,N2,O1,C,O2)
			 *      elseedge(N1,N2)
			 */
			class OpASP : public IOperator{
			private:
				// preprocessing
				void parseParameters(OperatorArguments& parameters, int& maxint, dlvhex::Program& program, dlvhex::AtomSet& facts);

			public:
				virtual std::string getName();
				virtual std::string getInfo();
				virtual std::set<std::string> getRecognizedParameters();
				virtual HexAnswer apply(bool debug, int arity, std::vector<HexAnswer*>& answers, OperatorArguments& parameters) throw (OperatorException);
			};
		}
	}
}


/*! \fn void parseParameters(dlvhex::Program& program, dlvhex::AtomSet& facts);
 * Parses the following parameters:
 * 	-) program: "some program code"
 * 	-) filefile: "some filename"
 * 	-) maxint: maximum integer value to be passed to the reasoner
 * \param program Reference to the program to append
 * \param facts Reference to the facts part of the program to append
 */


#endif
