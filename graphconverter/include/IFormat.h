#ifndef _IFORMAT_H_
#define _IFORMAT_H_

#include <DecisionDiagram.h>

#include <string>

using namespace dlvhex::dd::util;

namespace dlvhex{
	namespace dd{
		namespace tools{
			namespace graphconverter{

				/**
				 * \brief Implements functions for reading and writing decision diagrams in a certain file format.
				 */
				class IFormat{
				public:
					virtual std::string getName() = 0;
					virtual std::string getNameAbbr();

					virtual DecisionDiagram* read() throw (DecisionDiagram::InvalidDecisionDiagram) = 0;
					virtual void write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram) = 0;
				};
			}
		}
	}
}

#endif

/*! \fn virtual std::string dlvhex::dd::tools::graphconverter::IFormat::getName() = 0
 *  \brief Needs to return the (unique!) name of this format
 *  \return std::string The name of this file format
 */

/*! \fn virtual std::string dlvhex::dd::tools::graphconverter::IFormat::getNameAbbr()
 *  \brief Needs to return the desired (unique!) abbreviation of the name of this format. By default, this will be the name itself.
 *  \return std::string The short name of this file format
 */

/*! \fn virtual DecisionDiagram* dlvhex::dd::tools::graphconverter::IFormat::read() throw (DecisionDiagram::InvalidDecisionDiagram) = 0;
 *  \brief Reads a decision diagram from standard input.
 *  \return DecisionDiagram* A pointer to the read diagram
 *  \throw DecisionDiagram::InvalidDecisionDiagram In case of an error
 */

/*! \fn virtual void dlvhex::dd::tools::graphconverter::IFormat::write(DecisionDiagram*) throw (DecisionDiagram::InvalidDecisionDiagram) = 0;
 *  \brief Writes a decision diagram to standard output.
 *  \param dd* A pointer to the diagram to write
 *  \throw DecisionDiagram::InvalidDecisionDiagram In case of an error
 */
