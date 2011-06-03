#ifndef __OPASP_H_
#define __OPASP_H_

#include "IOperator.h"

#include <dlvhex/Program.h>
#include <dlvhex/AtomSet.h>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging;

namespace dlvhex{
	namespace merging{
		namespace plugin{
			/**
			 * This class implements an operator which allows for calling ASP programs which reason over decision diagrams.
			 * Usage:
			 * &operator["asp", A, K](A)
			 *	A(H1), ..., A(Hn)	... Handles to n sets of decision diagrams
			 *	K(program, c)		... c = arbitrary ASP code
			 *	K(file, f)		... f = the name of a file with ASP code
			 */
			class OpASP : public IOperator{
			private:
				// helper methods
std::string findUniqueAtomName(const std::string prefix, std::set<std::string>& usedPredNames);

				// preprocessing
				void parseParameters(int& maxint, dlvhex::Program& program, dlvhex::AtomSet& facts);

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
