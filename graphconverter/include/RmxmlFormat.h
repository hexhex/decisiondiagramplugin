#ifndef _RMXMLFORMAT_H_
#define _RMXMLFORMAT_H_

#include <IFormat.h>

#include <tinyxml.h>

using namespace dlvhex::dd::util;

namespace dlvhex{
	namespace dd{
		namespace tools{
			namespace graphconverter{

				class RmxmlFormat : public IFormat{
				private:
					// XML --> DD
					DecisionDiagram getDDDiag(TiXmlDocument* doc);
					DecisionDiagram::Node* getDDnode(DecisionDiagram* dd, TiXmlElement* ce);
					DecisionDiagram::Edge* getDDedge(DecisionDiagram* dd, DecisionDiagram::Node* parent, DecisionDiagram::Node* child, TiXmlElement* con);

					// DD --> XML
					TiXmlDocument getXmlDiag(DecisionDiagram* dd);
					TiXmlElement getXmlDiag(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id);
					TiXmlElement getXmlLeaf(DecisionDiagram* dd, DecisionDiagram::Node* ddnode, int& id);
					TiXmlElement getXmlInner(DecisionDiagram* dd, DecisionDiagram::Node* ddnode, int& id);
					TiXmlElement getXmlAttributeList(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id);
					TiXmlElement getXmlNormalAttributeList(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id, int& attrIndex);
					TiXmlElement getXmlClassificationAttributeList(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id, int& attrIndex);
					std::string getXmlCmpOperation(DecisionDiagram::Condition c);
				public:
					virtual std::string getName();
					virtual std::string getNameAbbr();
					virtual std::vector<DecisionDiagram*> read() throw (DecisionDiagram::InvalidDecisionDiagram);
					virtual void write(std::vector<DecisionDiagram*> dd) throw (DecisionDiagram::InvalidDecisionDiagram);
				};
			}
		}
	}
}

#endif
