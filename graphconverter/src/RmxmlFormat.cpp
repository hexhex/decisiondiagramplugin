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

DecisionDiagram* RmxmlFormat::read() throw (DecisionDiagram::InvalidDecisionDiagram){
	// load source document
	TiXmlDocument doc;

	if (doc.LoadFile(stdin)){
		try{
			DecisionDiagram dd = getDDDiag(&doc);
			return new DecisionDiagram(dd);
		}catch(...){
			throw DecisionDiagram::InvalidDecisionDiagram("Error: XML document is well-formed, but does not encode a valid decision tree.");
		}
	}else{
		throw DecisionDiagram::InvalidDecisionDiagram(std::string("Error: Could not parse stdin as XML document: ") + doc.ErrorDesc());
	}
}

void RmxmlFormat::write(DecisionDiagram* dd) throw (DecisionDiagram::InvalidDecisionDiagram){
	if (!dd->isTree()){
		throw DecisionDiagram::InvalidDecisionDiagram("Error: Could not write diagram. It must be a tree.");
	}

	try{
		TiXmlDocument doc = getXmlDiag(dd); 
		doc.Print();
	}catch(DecisionDiagram::InvalidDecisionDiagram idd){
		throw idd;
	}catch(...){
		throw DecisionDiagram::InvalidDecisionDiagram("Unknown error while writing decision diagram");
	}
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

	// some header information
	TiXmlElement root("object-stream");
	TiXmlElement treemodel("TreeModel");
	TiXmlElement source("source");
	source.InsertEndChild(TiXmlText("Decision Tree"));
	treemodel.InsertEndChild(source);
	treemodel.SetAttribute("id", ++id);

	// read model
	treemodel.InsertEndChild(getXmlDiag(dd, dd->getRoot(), id));

	// read attributes of model
	treemodel.InsertEndChild(getXmlAttributeList(dd, dd->getRoot(), id));

	// assemble the whole thing
	root.InsertEndChild(treemodel);
	doc.InsertEndChild(root);

	return doc;
}

// writes xml code for a decision diagram
TiXmlElement RmxmlFormat::getXmlDiag(DecisionDiagram* dd, DecisionDiagram::Node* ddnode, int& id){

	// add current element
	TiXmlElement* newElement;
	TiXmlElement root("root");
	TiXmlElement child("child");
	newElement = ddnode == dd->getRoot() ? &root : &child;
	newElement->SetAttribute("id", ++id);

	// inner node or leaf node?
	if (dynamic_cast<DecisionDiagram::LeafNode*>(ddnode) != NULL){
		// leaf node
		return getXmlLeaf(dd, ddnode, id);
	}else{
		// inner node
		return getXmlInner(dd, ddnode, id);
	}
}

// writes xml code for a leaf node
TiXmlElement RmxmlFormat::getXmlLeaf(DecisionDiagram* dd, DecisionDiagram::Node* ddnode, int& id){

	// leaf node
	TiXmlElement newElement(ddnode == dd->getRoot() ? "root" : "child");

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
	newElement.InsertEndChild(label);
	TiXmlElement children("children");
	newElement.InsertEndChild(children);

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
	newElement.InsertEndChild(distributionmap);
	return newElement;
}

// writes xml code for an inner node
TiXmlElement RmxmlFormat::getXmlInner(DecisionDiagram* dd, DecisionDiagram::Node* ddnode, int& id){

	// inner node
	TiXmlElement newElement(ddnode == dd->getRoot() ? "root" : "child");

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
		edge.InsertEndChild(getXmlDiag(dd, (*eit)->getTo(), id));

		children.InsertEndChild(edge);
	}
	newElement.InsertEndChild(children);
	return newElement;
}

// Writes the attribute list for a diagram (meta-attributes)
TiXmlElement RmxmlFormat::getXmlAttributeList(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id){
	// inner node

	// <headerExampleSet>
	// |
	// |---<attributes class="SimpleAttributes">
	//     |
	//     |---<attributes>
	//         |
	//         ...

	// some header information
	TiXmlElement headerExampleSet("headerExampleSet");
	headerExampleSet.SetAttribute("id", ++id);
	TiXmlElement simpleAttributes("attributes");
	simpleAttributes.SetAttribute("class", "SimpleAttributes");
	simpleAttributes.SetAttribute("id", ++id);

	// add simple and special attributes
	int attrIndex = 0;
	TiXmlElement attrib = getXmlNormalAttributeList(dd, node, id, attrIndex);
	attrib.InsertEndChild(getXmlClassificationAttributeList(dd, node, id, attrIndex));
	simpleAttributes.InsertEndChild(attrib);
	headerExampleSet.InsertEndChild(simpleAttributes);

	// some dummy elements that RapidMiner expects
	TiXmlElement statisticsMap("statisticsMap");
	statisticsMap.SetAttribute("id", ++id);
	TiXmlElement idMap("idMap");
	idMap.SetAttribute("id", ++id);
	headerExampleSet.InsertEndChild(statisticsMap);
	headerExampleSet.InsertEndChild(idMap);

	return headerExampleSet;
}

TiXmlElement RmxmlFormat::getXmlNormalAttributeList(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id, int& attrIndex){
	// <attributes class="SimpleAttributes">
	// |
	// |---<attributes>
	//     |
	//     |---<AttributeRole>
	//     |   |
	//     |   |---<special>
	//     |   |---<attribute>
	//     |       |---<attributeDescription>
	//     |       |   |
	//     |       |   |---<name>
	//     |       |   |---<valueType>
	//     |       |   |---<blockType>
	//     |       |   |---<index>
	//     |       |
	//     |       |---<transformations/>
	//     |       |---<statistics/>
	//     |
	//     |---<AttributeRole>
	//     |   |
	//     |    ...

	// extract the (normal) attributes
	std::set<std::string> attributes;
	std::set<DecisionDiagram::Edge*> edges = dd->getEdges();
	for (std::set<DecisionDiagram::Edge*>::iterator edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++){
		// only conditional edges contain conditions
		if (!dynamic_cast<DecisionDiagram::ElseEdge*>(*edgeIt)){
			attributes.insert(getXmlCmpAttribute((*edgeIt)->getCondition()));
		}
	}

	// some header information
	TiXmlElement attributesElement("attributes");
	attributesElement.SetAttribute("class", "linked-list");
	attributesElement.SetAttribute("id", ++id);

	// for all (normal) attributes
	for (std::set<std::string>::iterator attrIt = attributes.begin(); attrIt != attributes.end(); attrIt++){
		// Add the current attribute
		TiXmlElement attributeRole("AttributeRole");
		attributeRole.SetAttribute("id", ++id);
		//    it's not a special attribute
		TiXmlElement special("special");
		special.InsertEndChild(TiXmlText("false"));
		attributeRole.SetAttribute("id", ++id);
		attributeRole.InsertEndChild(special);
		//    the attributes' meta-attributes
		TiXmlElement attribute("attribute");
		attribute.SetAttribute("class", "NumericalAttribute");
		attribute.SetAttribute("id", ++id);

		//    fill in the meta-attributes of the current attribute: name, valueType, blockType, defaultValue, index
		TiXmlElement constructiondescription("constructionDescription");
		constructiondescription.InsertEndChild(TiXmlText(attrIt->c_str()));
		TiXmlElement description("attributeDescription");
		description.SetAttribute("id", ++id);
		TiXmlElement aName("name");
		aName.InsertEndChild(TiXmlText(attrIt->c_str()));
		TiXmlElement aValueType("valueType");
		aValueType.InsertEndChild(TiXmlText("3"));
		TiXmlElement aBlockType("blockType");
		aBlockType.InsertEndChild(TiXmlText("1"));
		TiXmlElement aDefaultValue("defaultValue");
		aDefaultValue.InsertEndChild(TiXmlText("0.0"));
		TiXmlElement aIndex("index");
		aIndex.InsertEndChild(TiXmlText(StringHelper::toString(++attrIndex).c_str()));

		// assemble the whole thing
		description.InsertEndChild(aName);
		description.InsertEndChild(aValueType);
		description.InsertEndChild(aBlockType);
		description.InsertEndChild(aDefaultValue);
		description.InsertEndChild(aIndex);
		attribute.InsertEndChild(constructiondescription);
		attribute.InsertEndChild(description);
		TiXmlElement transformations("transformations");
		transformations.SetAttribute("id", ++id);
		attribute.InsertEndChild(transformations);
		TiXmlElement statistics("statistics");
		statistics.SetAttribute("id", ++id);
		statistics.SetAttribute("class", "linked-list");
		attribute.InsertEndChild(statistics);
		attributeRole.InsertEndChild(attribute);

		// finally insert the attribute
		attributesElement.InsertEndChild(attributeRole);
	}
	return attributesElement;
}

TiXmlElement RmxmlFormat::getXmlClassificationAttributeList(DecisionDiagram* dd, DecisionDiagram::Node* node, int& id, int& attrIndex){

	// extract the occurring class labels
	std::set<std::string> classes;
	try{
		std::set<DecisionDiagram::LeafNode*> leafs = dd->getLeafNodes();
		std::map<std::string, int> cmap = StringHelper::extractDistribution((*leafs.begin())->getClassification());
		for (std::map<std::string, int>::iterator classIt = cmap.begin(); classIt != cmap.end(); classIt++){
			classes.insert(classIt->first);
		}
	}catch(StringHelper::NotContainedException nce){
		throw DecisionDiagram::InvalidDecisionDiagram("At least one of the leaf nodes does not contain a class frequency distribution. This is mandatory for RapidMiner.");
	}

	// now we add the special attribute for the classification
	// <AttributeRole>
	// |
	// |---<special>true</special>
	// |---<specialName>label</specialName>
	// |---<attribute class="PolynominalMapping">
	//     |---<nominalMapping>
	//     |   |---<symbolToIndexMap>
	//     |       |---<entry>
	//     |       |   |
	//     |       |   |---<string>
	//     |       |   |---<int>
	//     |       |
	//     |       |---<entry>
	//     |       |   |
	//     |       |   |---<string>
	//     |       |   |---<int>
	//     |       |
	//     |       | ...
	//     |
	//     |---<attributeDescription>
	//     |   |
	//     |   |---<name>
	//     |   |---<valueType>
	//     |   |---<blockType>
	//     |   |---<index>
	//     |
	//     |---<transformations/>
	//     |---<statistics/>

	TiXmlElement attributeRole("AttributeRole");
	attributeRole.SetAttribute("id", ++id);
	//    it's a special attribute
	TiXmlElement special("special");
	special.InsertEndChild(TiXmlText("true"));
	attributeRole.SetAttribute("id", ++id);
	attributeRole.InsertEndChild(special);
	//    its role is "label"
	TiXmlElement role("specialName");
	role.InsertEndChild(TiXmlText("label"));
	attributeRole.InsertEndChild(role);

	//    the attributes' meta-attributes
	TiXmlElement attribute("attribute");
	attribute.SetAttribute("class", "PolynominalAttribute");
	attribute.SetAttribute("id", ++id);
	TiXmlElement description("attributeDescription");
	description.SetAttribute("id", ++id);
	TiXmlElement aName("name");
	aName.InsertEndChild(TiXmlText("classification"));
	TiXmlElement aValueType("valueType");
	aValueType.InsertEndChild(TiXmlText("1"));
	TiXmlElement aBlockType("blockType");
	aBlockType.InsertEndChild(TiXmlText("1"));
	TiXmlElement aDefaultValue("defaultValue");
	aDefaultValue.InsertEndChild(TiXmlText("0.0"));
	TiXmlElement aIndex("index");
	aIndex.InsertEndChild(TiXmlText(StringHelper::toString(++attrIndex).c_str()));

	// add all occurring classes: create mappings from indices to symbols and the other way round
	TiXmlElement nominalMapping("nominalMapping");
	nominalMapping.SetAttribute("class", "PolynominalMapping");
	nominalMapping.SetAttribute("id", ++id);
	TiXmlElement symbolToIndexMap("symbolToIndexMap");
	symbolToIndexMap.SetAttribute("id", ++id);
	TiXmlElement indexToSymbolMap("indexToSymbolMap");
	indexToSymbolMap.SetAttribute("id", ++id);
	int cIndex = 0;
	for (std::set<std::string>::iterator cIt = classes.begin(); cIt != classes.end(); cIt++){
		TiXmlElement entry("entry");
		TiXmlElement string("string");
		TiXmlElement _int("int");
		string.InsertEndChild(TiXmlText(cIt->c_str()));
		_int.InsertEndChild(TiXmlText(StringHelper::toString(cIndex++).c_str()));
		entry.InsertEndChild(string);
		entry.InsertEndChild(_int);
		symbolToIndexMap.InsertEndChild(entry);
		indexToSymbolMap.InsertEndChild(string);
	}

	// some dummy elements that RapidMiner expects
	TiXmlElement transformations("transformations");
	transformations.SetAttribute("id", ++id);
	TiXmlElement statistics("statistics");
	statistics.SetAttribute("id", ++id);
	statistics.SetAttribute("class", "linked-list");

	// assemble the whole thing
	description.InsertEndChild(aName);
	description.InsertEndChild(aValueType);
	description.InsertEndChild(aBlockType);
	description.InsertEndChild(aDefaultValue);
	description.InsertEndChild(aIndex);
	nominalMapping.InsertEndChild(symbolToIndexMap);
	nominalMapping.InsertEndChild(indexToSymbolMap);
	attribute.InsertEndChild(nominalMapping);
	attribute.InsertEndChild(description);
	attribute.InsertEndChild(transformations);
	attribute.InsertEndChild(statistics);
	attributeRole.InsertEndChild(attribute);

	return attributeRole;
}

// Finds out the attribute of the two operands and returns it
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

// Finds out the numeric value of the two operands and returns it
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

	if (rootnode){
		dd.setRoot(rootnode);
		return dd;
	}else{
		throw DecisionDiagram::InvalidDecisionDiagram("Could not detect the root node of the decision diagram");
	}
}

// translates a node-element into a DD-node
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

// Translates an edge-element into a DD-edge
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
