#include <RmxmlFormat.h>
#include <StringHelper.h>

#include <map>
#include <iostream>
#include <string>
#include <sstream>

std::string RmxmlFormat::getName(){
	return "rmxml";
}

std::string RmxmlFormat::getNameAbbr(){
	return "xml";
}

DecisionDiagram* RmxmlFormat::read(){
	// load source document
	TiXmlDocument doc;

	if (doc.LoadFile(stdin)){
		try{
			DecisionDiagram dd = getDDDiag(&doc);
			return new DecisionDiagram(dd);
		}catch(...){
			std::cout << "XML document is well-formed, but does not encode a valid decision tree." << std::endl;
			return NULL;
		}
	}else{
		std::cout << "Could not parse stdin as XML document: " << doc.ErrorDesc() << std::endl;
		return NULL;
	}
}

bool RmxmlFormat::write(DecisionDiagram* dd){
	if (!dd->isTree()){
		std::cerr << "Could not write diagram. It must be a tree." << std::endl;
		return false;
	}

	TiXmlDocument doc = getXmlDiag(dd); 
	doc.Print();

	return true;
}

// ============================== DD --> XML ==============================

TiXmlDocument RmxmlFormat::getXmlDiag(dlvhex::dd::DecisionDiagram* dd){
	TiXmlDocument doc;

	int id = 0;

	// <object-stream>
	// |
	// |---<TreeModel>
	//     |---<source>
	//     |---<root>
	//         |
	//         |---<children>
	//             |
	//             ...

	TiXmlElement root("object-stream");
	TiXmlElement treemodel("TreeModel");
	TiXmlElement source("source");
	source.InsertEndChild(TiXmlText("Decision Tree"));
	treemodel.InsertEndChild(source);
	treemodel.SetAttribute("id", ++id);

	getXmlDiag(dd, dd->getRoot(), &treemodel, id);

	root.InsertEndChild(treemodel);
	doc.InsertEndChild(root);

	return doc;
}

void RmxmlFormat::getXmlDiag(DecisionDiagram* dd, DecisionDiagram::Node* ddnode, TiXmlElement* parent, int& id){

	// add current element
	TiXmlElement* newElement;
	TiXmlElement root("root");
	TiXmlElement child("child");
	newElement = ddnode == dd->getRoot() ? &root : &child;
	newElement->SetAttribute("id", ++id);

	// inner node or leaf node?
	if (dynamic_cast<DecisionDiagram::LeafNode*>(ddnode) != NULL){
		// leaf node

		// <root>/<child>
		// |
		// |---<label>
		// |---<children/>
		// |---<counterMap>
		// |---<counterMap>
		//     |
		//     |---<entry>
		//     |   |
		//     |   |---<string>
		//     |   |---<int>
		//     |
		//     |---<entry>
		//     |   |
		//     |   |---<string>
		//     |   |---<int>
		//     ...

		DecisionDiagram::LeafNode* leaf = dynamic_cast<DecisionDiagram::LeafNode*>(ddnode);

		// store classification at this leaf node
		TiXmlElement label("label");
		label.InsertEndChild(TiXmlText(StringHelper::extractClass(leaf->getClassification()).c_str()));
		newElement->InsertEndChild(label);
		TiXmlElement children("children");
		newElement->InsertEndChild(children);

		// built the distribution map
		TiXmlElement distributionmap("counterMap");
		std::map<std::string, int> distmap = StringHelper::extractDistribution(leaf->getClassification());
		for (std::map<std::string, int>::iterator distValue = distmap.begin(); distValue != distmap.end(); distValue++){
			TiXmlElement entry("entry");
			TiXmlElement _string("string");
			TiXmlElement _int("int");
			_string.InsertEndChild(TiXmlText(distValue->first.c_str()));
			_int.InsertEndChild(TiXmlText(StringHelper::toString(distValue->second).c_str()));
			entry.InsertEndChild(_string);
			entry.InsertEndChild(_int);
			distributionmap.InsertEndChild(entry);
		}

		// extract classification from leaf node's label
		newElement->InsertEndChild(distributionmap);
	}else{
		// inner node

		// <root>/<child>
		// |
		// |---<children>
		//     |
		//     |---<edge>
		//     |   |
		//     |   |---<condition>
		//     |   |   |
		//     |   |   |---<attributeName>
		//     |   |   |---<value>
		//     |   |
		//     |   |---<child> (recursive call for this node!)
		//     |
		//     |---<edge>
		//     |   |
		//     |   |---<condition>
		//     |   |   |
		//     |   |   |---<attributeName>
		//     |   |   |---<value>
		//     |   |
		//     |   |---<child> (recursive call for this node!)
		//  	...

		// make node for the children
		TiXmlElement children("children");
		children.SetAttribute("class", "linked-list");
		children.SetAttribute("id", ++id);

		// add all children
		std::set<DecisionDiagram::Edge*> outEdges = ddnode->getOutEdges();
		for (std::set<DecisionDiagram::Edge*>::iterator eit = outEdges.begin(); eit != outEdges.end(); eit++){

			// make an edge to this child
			TiXmlElement edge("com.rapidminer.operator.learner.tree.Edge");
			edge.SetAttribute("id", ++id);

			// add the condition
			TiXmlElement condition("condition");
			condition.SetAttribute("id", ++id);
			DecisionDiagram::Condition ddcondition = (*eit)->getCondition();
			condition.SetAttribute("class", getXmlCmpOperation(ddcondition).c_str());
			TiXmlElement attribute("attributeName");
			attribute.InsertEndChild(TiXmlText(getXmlCmpAttribute(ddcondition).c_str()));
			TiXmlElement value("value");
			value.InsertEndChild(TiXmlText(getXmlCmpValue(ddcondition).c_str()));
			condition.InsertEndChild(value);
			condition.InsertEndChild(attribute);

			// add the child itself (recursively)
			edge.InsertEndChild(condition);
			getXmlDiag(dd, (*eit)->getTo(), &edge, id);

			children.InsertEndChild(edge);
		}
		newElement->InsertEndChild(children);
	}
	// add this node to it's parent
	parent->InsertEndChild(TiXmlElement(*newElement));
}

std::string RmxmlFormat::getXmlCmpAttribute(DecisionDiagram::Condition c){
	try{
		// check if the first operand is a number
		StringHelper::atof(c.getOperand1());

		// yes: then the second operand is the attribute (check if it is NO number!)
		try{
			StringHelper::atof(c.getOperand2());
			throw DecisionDiagram::InvalidDecisionDiagram(std::string("None of the operands in condition \"") + c.toString() + "\" is an attribute (both are numbers)");
		}catch(StringHelper::NotContainedException nce){}

		return c.getOperand2();
	}catch(StringHelper::NotContainedException nce){
		// no, then it's the attribute
		return c.getOperand1();
	}
}

std::string RmxmlFormat::getXmlCmpValue(DecisionDiagram::Condition c){
	try{
		// check if the first operand is a number
		StringHelper::atof(c.getOperand1());

		// yes: then it's the compare value
		return c.getOperand1();
	}catch(StringHelper::NotContainedException nce){
		// no, then it's the second operand (must be a float value - check this!)
		try{
			StringHelper::atof(c.getOperand2());
			return c.getOperand2();
		}catch(StringHelper::NotContainedException nce2){
			throw DecisionDiagram::InvalidDecisionDiagram(std::string("None of the operands in condition \"") + c.toString() + "\" is a float value");
		}
	}
}

std::string RmxmlFormat::getXmlCmpOperation(DecisionDiagram::Condition c){
	// Check if the first or the second operand is the attribute
	std::string attribute = getXmlCmpAttribute(c);
	bool negate = false;
	if (c.getOperand1() == attribute){
		// just take the comparison operator as it is
	}else{
		// negate the comparision operator since the arguments will be swapped
		negate = true;
	}

	// translate the comparison operator
	switch (c.getOperation()){
		case DecisionDiagram::Condition::lt: return negate ? "com.rapidminer.operator.learner.tree.GreaterSplitCondition" : "com.rapidminer.operator.learner.tree.LessSplitCondition";
		case DecisionDiagram::Condition::le: return negate ? "com.rapidminer.operator.learner.tree.GreaterEqualsSplitCondition" : "com.rapidminer.operator.learner.tree.LessEqualsSplitCondition";
		case DecisionDiagram::Condition::eq: return "com.rapidminer.operator.learner.tree.EqualsSplitCondition";
		case DecisionDiagram::Condition::ge: return negate ? "com.rapidminer.operator.learner.tree.LessEqualsSplitCondition" : "com.rapidminer.operator.learner.tree.GreaterEqualsSplitCondition";
		case DecisionDiagram::Condition::gt: return negate ? "com.rapidminer.operator.learner.tree.LessSplitCondition" : "com.rapidminer.operator.learner.tree.GreaterSplitCondition";
		case DecisionDiagram::Condition::else_: throw DecisionDiagram::InvalidDecisionDiagram(std::string("Diagrams containing else edges cannot be written in rmxml format"));
		default: assert(0);
	}
}


// ============================== XML --> DD ==============================

DecisionDiagram RmxmlFormat::getDDDiag(TiXmlDocument* ce){

	TiXmlHandle hDoc(ce);

	DecisionDiagram dd;

	DecisionDiagram::Node* rootnode = getDDnode(&dd, hDoc.FirstChild("object-stream").FirstChild("TreeModel").FirstChild("root").ToElement());

	dd.setRoot(rootnode);

	return dd;
}

DecisionDiagram::Node* RmxmlFormat::getDDnode(DecisionDiagram* dd, TiXmlElement* elem){

	// enumerate children
	if (!elem->FirstChild("label")){
		// inner node

		// <root>/<child>
		// |
		// |---<children>
		//     |
		//     |---<edge>
		//     |   |
		//     |   |---<condition>
		//     |   |   |
		//     |   |   |---<attributeName>
		//     |   |   |---<value>
		//     |   |
		//     |   |---<child> (recursive call for this node!)
		//     |
		//     |---<edge>
		//     |   |
		//     |   |---<condition>
		//     |   |   |
		//     |   |   |---<attributeName>
		//     |   |   |---<value>
		//     |   |
		//     |   |---<child> (recursive call for this node!)
		//  	...

		// create inner node
		DecisionDiagram::Node* node = dd->addNode(std::string("node") + std::string(elem->Attribute("id")));

		// subnodes
		TiXmlElement* child = elem->FirstChild("children")->FirstChild() ? elem->FirstChild("children")->FirstChild()->ToElement() : NULL;
		while (child){
			// read condition
			TiXmlElement* condition = child->FirstChild("condition")->ToElement();
			// read and create subnode
			DecisionDiagram::Node* subnode = getDDnode(dd, child->FirstChild("child")->ToElement());

			// connect the nodes using the given condition
			getDDedge(dd, node, subnode, condition);

			// goto next child
			child = child->NextSibling() ? child->NextSibling()->ToElement() : NULL;
		}

		return node;
	}else{
		// leaf node

		// <root>/<child>
		// |
		// |---<label>
		// |---<children/>
		// |---<counterMap>
		// |---<counterMap>
		//     |
		//     |---<entry>
		//     |   |
		//     |   |---<string>
		//     |   |---<int>
		//     |
		//     |---<entry>
		//     |   |
		//     |   |---<string>
		//     |   |---<int>
		//     ...

		// extract label
		std::string label = elem->FirstChild("label")->ToElement()->GetText();

		// extract distribution map
		TiXmlElement* entry = elem->FirstChild("counterMap")->FirstChild() ? elem->FirstChild("counterMap")->FirstChild()->ToElement() : NULL;
		std::stringstream map;
		bool first = true;
		while (entry){
			map << (first ? "" : ",") << entry->FirstChild("string")->ToElement()->GetText() << ":" << entry->FirstChild("int")->ToElement()->GetText();
			entry = entry->NextSibling() ? entry->NextSibling()->ToElement() : NULL;
			first = false;
		}
		// node label is the class plus the distribution
		label = label + std::string(" {") + map.str() + std::string("}");

		return dd->addLeafNode(std::string("node") + std::string(elem->Attribute("id")), label);
	}
}

DecisionDiagram::Edge* RmxmlFormat::getDDedge(DecisionDiagram* dd, DecisionDiagram::Node* parent, DecisionDiagram::Node* child, TiXmlElement* con){
	// extract operands
	std::string op = con->Attribute("class");
	std::string attr = std::string(con->FirstChild("attributeName")->ToElement()->GetText());
	std::string value = std::string(con->FirstChild("value")->ToElement()->GetText());

	// detect comparison operator
	DecisionDiagram::Condition::CmpOp _op;
	if (op == std::string("com.rapidminer.operator.learner.tree.GreaterSplitCondition"))		_op = DecisionDiagram::Condition::gt;
	else if (op == std::string("com.rapidminer.operator.learner.tree.LessSplitCondition"))		_op = DecisionDiagram::Condition::lt;
	else if (op == std::string("com.rapidminer.operator.learner.tree.GreaterEqualsSplitCondition"))	_op = DecisionDiagram::Condition::ge;
	else if (op == std::string("com.rapidminer.operator.learner.tree.LessEqualsSplitCondition"))	_op = DecisionDiagram::Condition::le;
	else if (op == std::string("com.rapidminer.operator.learner.tree.EqualsSplitCondition"))	_op = DecisionDiagram::Condition::eq;
	else throw DecisionDiagram::InvalidDecisionDiagram(std::string("Operator \"") + op + std::string("\" was not recognized"));

	// insert edge
	return dd->addEdge(parent, child, dlvhex::dd::DecisionDiagram::Condition(attr, value, _op));
}
