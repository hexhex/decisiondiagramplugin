#ifndef __OPUSERPREFERENCES_H_
#define __OPUSERPREFERENCES_H_

#include "dlvhex-mergingplugin/IOperator.h"
#include <OpMajorityVoting.h>
#include <DecisionDiagram.h>
#include <vector>

DLVHEX_NAMESPACE_USE

using namespace dlvhex::merging;

namespace dlvhex{
	namespace dd{

		/**
		 * This class implements the user preferences operator. It assumes that arbitrary many input decision trees are passed to the operator. Further it assumes
		 * user preferences about the decision classes to be specified in the parameters structure (as values in the key-value pairs).
		 * The preferences are evaluated in top-down manner (in the sourcecode). Thus, earlier decisions can be overwritten by later ones.
		 * Rules are expected in the following format:
		 *	class1 >> class2	or	class1 >Nr> class2
		 * The first expression states that class1 is _always_ preferred to class2. Thus, if at least one diagram delivers class1, it is selected.
		 * The second expression states that class1 is preferred to class2 if the voting difference (i.e. the number of diagrams) is at least Nr.
		 * In cases where these rules are not sufficient, the lexically smaller class label will be selected
		 * The result will be another decision diagram which gives preference to one of the classes in case of different classifications from the diagrams.
		 * Usage:
		 * &operator["userpreferences", DD](A)
		 *	DD	... predicate with handles to arbitrary many decision diagrams
		 *	A	... answer to the operator result
		 */
		class OpUserPreferences : public OpMajorityVoting{
		private:
			class UserPreference{
			private:
				std::string classP, classC;
				int minDiff;
				std::string trim(std::string s);
			public:
				class InvalidUserPreference{};
				UserPreference(std::string definition);
				std::string getPreferredClass();
				std::string getCmpClass();
				int getMinDiff();
			};
			typedef std::vector<UserPreference> UserPreferences;
			UserPreferences getUserPreferences(OperatorArguments& parameters);
		public:
			virtual std::string getName();
			virtual HexAnswer apply(int arity, std::vector<HexAnswer*>& arguments, OperatorArguments& parameters) throw (OperatorException);
		};
	}
}

#endif
