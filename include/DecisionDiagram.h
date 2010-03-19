#ifndef __DECISIONDIAGRAM_H_
#define __DECISIONDIAGRAM_H_

#include <dlvhex/AtomSet.h>
#include <vector>
#include <set>
#include <string>

DLVHEX_NAMESPACE_USE

namespace dlvhex{
	namespace dd{

		/**
		 * This class provides the data structures and methodes necessary to build decision diagrams from answer sets representing them.
		 */
		class DecisionDiagram{
		public:
			/**
			 * General exception class for errors in the context of decision diagrams.
			 */
			class InvalidDecisionDiagram{
			private:
				std::string msg;
			public:
				InvalidDecisionDiagram(std::string msg);
				std::string getMessage();
			};

			/*! \fn DecisionDiagram::InvalidDecisionDiagram::InvalidDecisionDiagram(std::string msg)
			 *  \brief Constructs a new exception with a certain error messasge.
			 *  \param msg The error message to use
			 */

			/*! \fn std::string DecisionDiagram::InvalidDecisionDiagram::getMessage()
			 *  \brief Retrieves the error message of this exception.
			 *  \param std::string The error message of this exception
			 */


			class Edge;

			/**
			 * A class representing one node of a decision diagram.
			 */
			class LeafNode;
			class Node{
			private:
				std::string label;		// This node's label
				std::set<Edge*> inEdges;	// All the ingoing edges this Node is involved in
				std::set<Edge*> outEdges;	// All the outgoing edges this Node is involved in

				// The following methods are only called by members of DecisionDiagram in order to maintain the decision diagram's integrity
				friend class DecisionDiagram;
				Node(std::string l);
				void addEdge(Edge* e);
				void removeEdge(Edge* e);
			public:
				virtual ~Node();

				void setLabel(std::string l);
				std::string getLabel() const;

				std::set<Edge*> getEdges() const;
				std::set<Edge*> getInEdges() const;
				std::set<Edge*> getOutEdges() const;
				int getEdgesCount() const;
				int getInEdgesCount() const;
				int getOutEdgesCount() const;
				Node* getChild(int index);
				int getChildCount() const;
				Edge* getOutEdge(int index);

				virtual std::string toString() const;

				virtual bool operator==(const Node& n2) const;
				virtual bool operator==(const LeafNode& n2) const;
				virtual bool operator!=(const Node& n2) const;
				virtual bool operator!=(const LeafNode& n2) const;
			};

			/*! \fn DecisionDiagram::Node::Node(std::string l)
			 *  \brief Construct a new node with a certain label.
			 *  \param l The label of the node
			 */

			/*! \fn DecisionDiagram::Node::~Node()
			 *  \brief Destructor
			 */

			/*! \fn void DecisionDiagram::Node::addEdge(Edge* e)
			 *  \brief Adds a new edge to this node. Note that the edge must be inzident with this node, otherwise an InvalidDecisionDiagram instance will be thrown.
			 *  \param e The edge to add
			 *  \throws InvalidDecisionDiagram If e is not inzident with this node.
			 */

			/*! \fn void DecisionDiagram::Node::removeEdge(Edge* e)
			 *  \brief Removes an edge from this node. Note that the edge must have been added previously.
			 *  \param e The edge to remove
			 *  \throws InvalidDecisionDiagram If e was not added to this node.
			 */

			/*! \fn void DecisionDiagram::Node::setLabel(std::string l)
			 *  \brief Changes the label of this node.
			 *  \param l The new label of the node
			 */

			/*! \fn std::string DecisionDiagram::Node::getLabel()
			 *  \brief Retrieves the label of this node.
			 *  \return std::string The label of the node
			 */

			/*! \fn std::set<Edge*> DecisionDiagram::LeafNode::getEdges() const
			 *  \brief Retrieves the edges inzident with this node.
			 *  \return std::set<Edge*> A set of all edges inzident with this node.
			 */

			/*! \fn std::set<Edge*> DecisionDiagram::Node::getOutEdges() const
			 *  \brief Retrieves the outgoing edges of this node.
			 *  \return std::set<Edge*> A set of outgoing edges of this node.
			 */

			/*! \fn std::set<Edge*> DecisionDiagram::Node::getInEdges() const
			 *  \brief Retrieves the ingoing edges of this node.
			 *  \return std::set<Edge*> A set of ingoing edges of this node.
			 */

			/*! \fn int DecisionDiagram::Node::getEdgesCount() const
			 *  \brief Returns the number of edges inzident with this node.
			 *  \return int The number of edges inzident with this node.
			 */

			/*! \fn int DecisionDiagram::Node::getOutEdgesCount() const
			 *  \brief Returns the number of outgoing edges of this node.
			 *  \return int The number of edges outgoing edges of this node.
			 */

			/*! \fn int DecisionDiagram::Node::getInEdgesCount() const
			 *  \brief Returns the number of ingoing edges of this node.
			 *  \return int The number of edges ingoing edges of this node.
			 */

			/*! \fn int DecisionDiagram::Node::getChildCount() const
			 *  \brief Returns the number of child nodes.
			 *  \return int The number of child nodes.
			 */

			/*! \fn Node* DecisionDiagram::Node::getChild(int index) const
			 *  \brief Returns a pointer to the child with the given index.
			 *  \param index The 0-based index of a child to retrieve.
			 *  \return Node* A pointer to the child with the given index
			 *  \throws InvalidDecisionDiagram If the index is out of bounds
			 */

			/*! \fn Node* DecisionDiagram::Node::getOutEdge(int index) const
			 *  \brief Returns a pointer to the outgoing edge with the given index.
			 *  \param index The 0-based index of an outgoing edge to retrieve.
			 *  \return Edge* A pointer to the outgoing edge with the given index
			 *  \throws InvalidDecisionDiagram If the index is out of bounds
			 */

			/*! \fn std::string DecisionDiagram::Node::toString()
			 *  \brief Returns a string representation of this node.
			 *  \return std::string The string representation of this node.
			 */

			/*! \fn virtual bool DecisionDiagram::Node::operator==(const Node& n2) const
			 * Compares this node with another one _semantically_. Two nodes are equivalent if they encode the same classifier.
			 *  \throws bool True iff the nodes are semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::Node::operator==(const LeafNode& n2) const
			 * Compares this node with another one _semantically_. Two nodes are equivalent if they encode the same classifier.
			 *  \throws bool True iff the nodes are semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::Node::operator!=(const Node& n2) const
			 * Compares this node with another one _semantically_. Two nodes are equivalent if they encode the same classifier.
			 *  \throws bool True iff the nodes are not semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::Node::operator!=(const LeafNode& n2) const
			 * Compares this node with another one _semantically_. Two nodes are equivalent if they encode the same classifier.
			 *  \throws bool True iff the nodes are not semantically equivalent.
			 */


			/**
			 * A class representing one leaf node of a decision diagram.
			 */
			class LeafNode : public Node{
			public:
				/*
				 * A class that - when derived - stores arbitrary data structures that can be appended to leaf nodes
				 */
				class Data{
				public:
					virtual ~Data();	// just to make this type polymorphic
				};
			private:
				std::string classification;
				Data* data;

				// The following methods are only called by members of DecisionDiagram in order to maintain the decision diagram's integrity
				friend class DecisionDiagram;
				LeafNode(std::string l, std::string c);
				LeafNode(std::string l, std::string c, Data* d);
			public:
				virtual ~LeafNode();
				std::string getClassification();
				Data* getData();
				void setClassification(std::string c);
				void setData(Data* d);

				virtual std::string toString() const;

				virtual bool operator==(const Node& n2) const;
				virtual bool operator==(const LeafNode& n2) const;
			};

			/*! \fn DecisionDiagram::LeafNode::LeafNode(std::string l, std::string c)
			 *  \brief Construct a new leaf node with a certain label and classification.
			 *  \param l The label of the leaf node
			 *  \param c The classification of the leaf node
			 */

			/*! \fn DecisionDiagram::LeafNode::LeafNode(std::string l, std::string c, Data* d)
			 *  \brief Construct a new leaf node with a certain label and classification plus an additional data entry.
			 *  \param l The label of the leaf node
			 *  \param c The classification of the leaf node
			 *  \param d The data field of this leaf node
			 */

			/*! \fn DecisionDiagram::LeafNode::~LeafNode()
			 *  \brief Destructor
			 */

			/*! \fn std::string DecisionDiagram::LeafNode::getClassification()
			 *  \brief Returns the classification of this leaf node.
			 *  \return std::string The classification of this leaf node.
			 */

			/*! \fn Data* DecisionDiagram::LeafNode::getData()
			 *  \brief Returns the data appended to this leaf node. Will be NULL by default.
			 *  \return Data* Data pointer
			 */

			/*! \fn void DecisionDiagram::LeafNode::setClassification(std::string c)
			 *  \brief Sets a new classification for this leaf node.
			 *  \param c The new classification
			 *  \return void
			 */

			/*! \fn void DecisionDiagram::LeafNode::setData(Data* d)
			 *  \brief Appends a data instance to this leaf node.
			 *  \param d A pointer to some data structure. Note: This class will _not_ overtake memory management for this pointer!
			 *  \param d A pointer to some instance of Data or a subclass.
			 */

			/*! \fn std::string DecisionDiagram::LeafNode::toString()
			 *  \brief Returns a string representation of this leaf node.
			 *  \return std::string The string representation of this leaf node.
			 */

			/*! \fn virtual bool DecisionDiagram::LeafNode::operator==(const Node& n2) const
			 * Compares this leaf node with another one _semantically_. Two nodes are equivalent if they encode the same classifier.
			 *  \throws bool True iff the nodes are semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::LeafNode::operator==(const LeafNode& n2) const
			 * Compares this leaf node with another one _semantically_. Two nodes are equivalent if they encode the same classifier.
			 *  \throws bool True iff the nodes are semantically equivalent.
			 */


			/**
			 * A class representing the condition of an edge of a decision diagram.
			 */
			class Condition{
			public:
				/**
				 * Defines the possible comparison operators.
				 */
				enum CmpOp{
					lt,
					le,
					eq,
					ge,
					gt,
					else_
				};
			private:
				std::string operand1;
				std::string operand2;
				CmpOp operation;
			public:
				Condition(std::string operand1_, std::string operand2_, CmpOp operation_);
				Condition(std::string operand1_, std::string operand2_, std::string operation_);

				virtual ~Condition();
				std::string getOperand1() const;
				std::string getOperand2() const;
				CmpOp getOperation() const;
				std::string getAttribute() const;
				float getCmpValue() const;

				static CmpOp stringToCmpOp(std::string operation_);
				static std::string cmpOpToString(CmpOp op);

				virtual std::string toString() const;

				virtual bool operator==(const Condition& c2) const;
				virtual bool operator!=(const Condition& c2) const;
			};

			/*! \fn DecisionDiagram::Condition::Condition(std::string operand1_, std::string operand2_, CmpOp operation_)
			 *  \brief Returns a string representation of this condition.
			 *  \param operand1_ An arbitrary operand of a range query (basically an arbitrary string)
			 *  \param operand2_ An arbitrary operand of a range query (basically an arbitrary string)
			 *  \param operation_ The comparison operator used in the range query
			 */

			/*! \fn DecisionDiagram::Condition::Condition(std::string operand1_, std::string operand2_, std::string operation_)
			 *  \brief Returns a string representation of this condition.
			 *  \param operand1_ An arbitrary operand of a range query (basically an arbitrary string)
			 *  \param operand2_ An arbitrary operand of a range query (basically an arbitrary string)
			 *  \param operation_ The comparison operator used in the range query given as string
			 *  \throws InvalidDecisionDiagram If the operation given as string is not a valid comparison operator (<, <=, =, >, >=)
			 */

			/*! \fn DecisionDiagram::Condition::~Condition()
			 *  \brief Destructor
			 */

			/*! \fn std::string DecisionDiagram::Condition::getOperand1() const
			 *  \brief Returns the first operand of the range query.
			 *  \return std::string The first operand of the range query
			 */

			/*! \fn std::string DecisionDiagram::Condition::getOperand2() const
			 *  \brief Returns the second operand of the range query.
			 *  \return std::string The second operand of the range query
			 */

			/*! \fn CmpOp DecisionDiagram::Condition::getOperation() const
			 *  \brief Returns the operation of the range query.
			 *  \return CmpOp The operation of the range query
			 */

			/*! \fn std::string DecisionDiagram::Condition::getAttribute() const
			 *  \brief Figures out the alphanumeric comparison attribute. If both operands are attributes (i.e. no numbers), operand1 will be returned. If none is an attribute, an exception is thrown.
			 *  \return std::string The alphanumeric attribute. In the expression "X <= 10", "X" will be returned.
			 */

			/*! \fn float DecisionDiagram::Condition::getCmpValue() const
			 *  \brief Figures out the numeric comparison value. If both operands are numbers (i.e. no attributes), operand2 will be returned. If none is a number, an exception is thrown.
			 *  \return std::string The alphanumeric attribute. In the expression "X <= 10", "10" will be returned.
			 */

			/*! \fn static CmpOp DecisionDiagram::Condition::stringToCmpOp(std::string operation_)
			 *  \brief Converts a string into an element of the enumeration CmpOp.
			 *  \param operation_ A comparison operator given as string
			 *  \return CmpOp The operation specified by the string operation_.
			 *  \throws InvalidDecisionDiagram If the operation given as string is not a valid comparison operator (<, <=, =, >, >=)
			 */

			/*! \fn static std::string DecisionDiagram::Condition::cmpOpToString(CmpOp op)
			 *  \brief Converts an element of the enumeration CmpOp into a string representation.
			 *  \param op A comparison operator
			 *  \return std::string A string representation of op.
			 */

			/*! \fn std::string DecisionDiagram::Condition::toString()
			 *  \brief Returns a string representation of this condition.
			 *  \return std::string The string representation of this condition.
			 */

			/*! \fn bool DecisionDiagram::Condition::operator==(const Condition& c2) const
			 * Compares this condition with another one _semantically_. Two conditions are equivalent if they encode the same classifier.
			 *  \throws bool True iff the conditions are semantically equivalent.
			 */

			/*! \fn bool DecisionDiagram::Condition::operator!=(const Condition& c2) const
			 * Compares this condition with another one _semantically_. Two conditions are equivalent if they encode the same classifier.
			 *  \throws bool True iff the conditions are not semantically equivalent.
			 */


			/**
			 * A class representing one edge of a decision diagram.
			 */
			class ElseEdge;
			class Edge{
			private:
				Node *from, *to;
				Condition condition;

				// The following methods are only called by members of DecisionDiagram in order to maintain the decision diagram's integrity
				friend class DecisionDiagram;
				Edge(Node *f, Node *t, Condition c);
			public:
				virtual ~Edge();

				Node* getFrom();
				Node* getTo();
				Condition getCondition() const;

				virtual std::string toString() const;

				virtual bool operator==(const Edge& e2) const;
				virtual bool operator==(const ElseEdge& e2) const;
				virtual bool operator!=(const Edge& e2) const;
				virtual bool operator!=(const ElseEdge& e2) const;
			};

			/*! \fn DecisionDiagram::Edge::Edge(Node *f, Node *t, Condition c)
			 *  \brief Construct a directed conditional edge from two nodes and one condition
			 *  \param f The source node of this edge
			 *  \param t The destination node of this edge
			 *  \param c The condition of this edge
			 */

			/*! \fn DecisionDiagram::Edge::~Edge()
			 *  \brief Destructor
			 */

			/*! \fn Node* DecisionDiagram::Edge::getFrom()
			 *  \brief Returns the source node of this directed edge.
			 *  \return Node* The source node of this edge
			 */

			/*! \fn Node* DecisionDiagram::Edge::getTo()
			 *  \brief Returns the destination node of this directed edge.
			 *  \return Node* The destination node of this edge
			 */

			/*! \fn Condition DecisionDiagram::Edge::getCondition() const
			 *  \brief Returns the condition of this conditional edge.
			 *  \return Condition The condition of this conditional edge
			 */

			/*! \fn std::string DecisionDiagram::Edge::toString()
			 *  \brief Returns a string representation of this edge.
			 *  \return std::string The string representation of this edge.
			 */

			/*! \fn virtual bool DecisionDiagram::Edge::operator==(const Edge& e2) const
			 * Compares this edge with another one _semantically_. Two edges are equivalent if they encode the same classifier.
			 *  \throws bool True iff the edges are semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::Edge::operator==(const ElseEdge& e2) const
			 * Compares this edge with another one _semantically_. Two edges are equivalent if they encode the same classifier.
			 *  \throws bool True iff the edges are semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::Edge::operator!=(const Edge& e2) const
			 * Compares this edge with another one _semantically_. Two edges are equivalent if they encode the same classifier.
			 *  \throws bool True iff the edges are not semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::Edge::operator!=(const ElseEdge& e2) const
			 * Compares this edge with another one _semantically_. Two edges are equivalent if they encode the same classifier.
			 *  \throws bool True iff the edges are not semantically equivalent.
			 */


			/**
			 * A class representing one unconditional (else) edge of a decision diagram.
			 */
			class ElseEdge : public Edge{
			private:
				// The following methods are only called by members of DecisionDiagram in order to maintain the decision diagram's integrity
				friend class DecisionDiagram;
				ElseEdge(Node *f, Node *t);
			public:
				virtual ~ElseEdge();

				virtual std::string toString() const;

				virtual bool operator==(const Edge& e2) const;
				virtual bool operator==(const ElseEdge& e2) const;
			};

			/*! \fn DecisionDiagram::ElseEdge::ElseEdge(Node *f, Node *t)
			 *  \brief Construct a directed unconditional (else) edge from two nodes
			 *  \param f The source node of this edge
			 *  \param t The destination node of this edge
			 */

			/*! \fn DecisionDiagram::ElseEdge::~Edge()
			 *  \brief Destructor
			 */

			/*! \fn std::string DecisionDiagram::ElseEdge::toString()
			 *  \brief Returns a string representation of this unconditional (else) edge.
			 *  \return std::string The string representation of this unconditional (else) edge.
			 */

			/*! \fn virtual bool DecisionDiagram::ElseEdge::operator==(const Edge& e2) const
			 * Compares this else edge with another one _semantically_. Two edges are equivalent if they encode the same classifier.
			 *  \throws bool True iff the edges are semantically equivalent.
			 */

			/*! \fn virtual bool DecisionDiagram::ElseEdge::operator==(const ElseEdge& e2) const
			 * Compares this else edge with another one _semantically_. Two edges are equivalent if they encode the same classifier.
			 *  \throws bool True iff the edges are semantically equivalent.
			 */


		private:
			std::set<Node*> nodes;
			std::set<Edge*> edges;
			Node* root;

		public:
			// Constructors and destructors
			DecisionDiagram();
			DecisionDiagram(const DecisionDiagram &dd2);
			DecisionDiagram(AtomSet as);
			~DecisionDiagram();
			void clear();

			DecisionDiagram& operator=(const DecisionDiagram &dd2);

			// Modification
			Node* addNode(std::string label);
			LeafNode* addLeafNode(std::string label, std::string classification);
			Node* addNode(Node* template_);
			void removeNode(Node *n);
			void removeNode(Node *n, bool forceRemoveEdges);

			Edge* addEdge(Node* from, Node* to, Condition c);
			Edge* addEdge(std::string from, std::string to, Condition c);
			ElseEdge* addElseEdge(Node* from, Node* to);
			ElseEdge* addElseEdge(std::string from, std::string to);
			Edge* addEdge(Edge* template_);
			void removeEdge(Edge* e);
			Node* addDecisionDiagram(DecisionDiagram* dd2);
			Node* partialAddDecisionDiagram(DecisionDiagram *dd2, Node *n);

			void setRoot(Node* root);
			void useUniqueLabels(DecisionDiagram* dd2);
			std::string getUniqueLabel(std::string proposal) const;

			// Getter
			std::set<Node*> getNodes() const;
			std::set<LeafNode*> getLeafNodes() const;
			std::set<Edge*> getEdges() const;
			Node* getRoot() const;
			Node* getNodeByLabel(std::string label) const;
			int nodeCount() const;
			int leafCount() const;
			int edgeCount() const;

			// Advanced
			std::vector<Node*> containsCycles() const;
			bool isTree() const;
			virtual bool operator==(const DecisionDiagram &dd2) const;
			virtual bool operator!=(const DecisionDiagram &dd2) const;
			bool containsPath(const Node* from, const Node* to) const;


			// Output generation
			AtomSet toAnswerSet() const;
			std::string toString() const;
		};
	}
}

#endif


/*! \fn DecisionDiagram::DecisionDiagram()
 *  \brief Creates an empty decision diagram.
 */

/*! \fn DecisionDiagram::DecisionDiagram(const DecisionDiagram &dd2)
 *  \brief Copy-constructor
 *  \param dd2 Template decision diagram
 */

/*! \fn DecisionDiagram::DecisionDiagram(AtomSet as)
 *  \brief Constructs a decision diagram from an answer set. The predicates root(Name), innernode(Label), leafnode(Label, Classification), edge(Node1, Node2, Operand1, comparisonOperator, Operand2) and elseedge(Node1, Node2) will be interpreted. During construction some validity checks (e.g. usage of undefined nodes, cycle checks, etc.) will be performed and an instance of InvalidDecisionDiagram will be thrown in case of an error.
 *  \param as An answer set which defines a decision diagram using the following predicates: root(Name), innernode(Label), leafnode(Label, Classification), edge(Node1, Node2, Operand1, comparisonOperator, Operand2), elseedge(Node1, Node2).
 *  \throws InvalidDecisionDiagram The message text will describe the cause of the error.
 */

/*! \fn DecisionDiagram::~DecisionDiagram()
 * Destructor. Frees allocated memory.
 */

/*! \fn void DecisionDiagram::empty()
 * Removes all nodes and edges from this decision diagram.
 */

/*! \fn DecisionDiagram& DecisionDiagram::operator=(const DecisionDiagram &dd2)
 * Assignment operator. This decision diagram will be overwritten by dd2. All the internal structures (nodes, edges) of dd2 will be copied and pointers will be adjusted accordingly. Thus dd2 and this decision diagram will be completely independent after assignment, i.e. modifications in one of the diagrams will not have any effects on the other one.
 *  \param dd2 A template for the assignment.
 *  \return DecisionDiagram& Reference to this decision diagram after assignment.
 */

/*! \fn Node* DecisionDiagram::addNode(std::string label)
 * Adds a node with a certain label to this decision diagram. Note that the label must be unique.
 *  \param label The label for the new node. If the label is not unique, an instance of InvalidDecisionDiagram will be thrown.
 *  \ŗeturn Node* A pointer to the added node.
 *  \throws InvalidDecisionDiagram If the node label is not unique
 */

/*! \fn LeafNode* DecisionDiagram::addLeafNode(std::string label, std::string classification)
 * Adds a leaf node with a certain label and classification to this decision diagram. Note that the label must be unique.
 *  \param label The label for the new node. If the label is not unique, an instance of InvalidDecisionDiagram will be thrown.
 *  \param classification The classification of this leaf node (basically an arbitrary string).
 *  \ŗeturn LeafNode* A pointer to the added leaf node.
 *  \throws InvalidDecisionDiagram If the node label is not unique
 */

/*! \fn Node* DecisionDiagram::addNode(Node* template_)
 * Adds a new node (can be inner or leaf) from a given template. The new node will be exactly the same as the template.
 *  \param template_ Another node which acts as a template for the new one. The new node will have the same attribute values as the template. Note that a copy is created, i.e. the new node and the template will be independent of each other.
 *  \ŗeturn Node* A pointer to the added node.
 *  \throws InvalidDecisionDiagram If the node label is not unique
 */

/*! \fn void DecisionDiagram::removeNode(Node* n)
 * Removes a node from this decision diagram.
 *  \param n A pointer to the node to remove (must be a pointer to exactly the node to remove, not only to one with the same attributes!)
 *  \throws InvalidDecisionDiagram If n is not part of this decision diagram or if n has inzident edges (which must be removed first)
 */

/*! \fn void DecisionDiagram::removeNode(Node* n, bool forceRemoveEdges)
 * Removes a node from this decision diagram.
 *  \param n A pointer to the node to remove (must be a pointer to exactly the node to remove, not only to one with the same attributes!)
 *  \param forceRemoveEdges If true, inzident edges of n will be removed too. Otherwise, removal will be denied if n has inzident edges.
 *  \throws InvalidDecisionDiagram If n is not part of this decision diagram or if n has inzident edges and forceRemoveEdges is false
 */

/*! \fn Edge* DecisionDiagram::addEdge(Node* from, Node* to, Condition c)
 * Adds a directed conditional or unconditional edge between two nodes of this decision diagram.
 *  \param from A pointer to a node of this decision diagram acting as the edge's source. Note that it must be a pointer to exactly the node to use, not only one with the same attribute values.
 *  \param to A pointer to a node of this decision diagram acting as the edge's destination. Note that it must be a pointer to exactly the node to use, not only one with the same attribute values.
 *  \param c The condition of the edge. If Condition::else_ is used as operation, an else edge will be created.
 *  \return Edge* A pointer to the added edge
 *  \throws InvalidDecisionDiagram If one of the endpoints is not part of this decision diagram
 */

/*! \fn Edge* DecisionDiagram::addEdge(std::string from, std::string to, Condition c)
 * Adds a directed conditional edge between two nodes of this decision diagram.
 *  \param from The label of the node acting as the edge's source.
 *  \param to The label of the node acting as the edge's destination.
 *  \param c The condition of the edge
 *  \return Edge* A pointer to the added edge
 *  \throws InvalidDecisionDiagram If one of the endpoints is not part of this decision diagram
 */

/*! \fn Edge* DecisionDiagram::addElseEdge(Node* from, Node* to)
 * Adds a directed unconditional (else) edge between two nodes of this decision diagram.
 *  \param from A pointer to a node of this decision diagram acting as the edge's source. Note that it must be a pointer to exactly the node to use, not only one with the same attribute values.
 *  \param to A pointer to a node of this decision diagram acting as the edge's destination. Note that it must be a pointer to exactly the node to use, not only one with the same attribute values.
 *  \return Edge* A pointer to the added edge
 *  \throws InvalidDecisionDiagram If one of the endpoints is not part of this decision diagram
 */

/*! \fn Edge* DecisionDiagram::addElseEdge(std::string from, std::string to)
 * Adds a directed unconditional (else) edge between two nodes of this decision diagram.
 *  \param from The label of the node acting as the edge's source.
 *  \param to The label of the node acting as the edge's destination.
 *  \return Edge* A pointer to the added edge
 *  \throws InvalidDecisionDiagram If one of the endpoints is not part of this decision diagram
 */

/*! \fn Edge* DecisionDiagram::addElseEdge(Edge* template_)
 * Adds a directed (conditional or unconditional) edge between two nodes of this decision diagram. The edge is constructed from a template. The edge to be inserted will be semantically equivalent to the template. However, pointers to the endpoints will be adjusted accordingly, such that not the nodes in the template, but the analougous nodes in this decision diagram (with the same label) will be referenced. The new edge and the template will be completely independent.
 *  \param template_ The template to construct the edge from.
 *  \return Edge* A pointer to the added edge
 *  \throws InvalidDecisionDiagram If one of the endpoints is not part of this decision diagram
 */

/*! \fn void DecisionDiagram::removeEdge(Edge* e)
 * Removes an edge from the decision diagram.
 *  \param e A pointer to the edge to remove. Note that it must be a pointer to exactly the edge to remove, not just to one with the same attribute values.
 *  \throws InvalidDecisionDiagram If e is not part of this decision diagram
 */

/*! \fn Node* DecisionDiagram::addDecisionDiagram(DecisionDiagram* dd2)
 * Unions this decision diagram with dd2.
 *  \param dd2 A second decision diagram to be added to this one. The caller has to make sure that the node labels of dd2 are distinct from the labels in this diagram.
 *  \return Node* A pointer to the root node of the added decision diagram within this decision diagram.
 *  \throws InvalidDecisionDiagram If dd2 and this decision diagram have node labels in common.
 */

/*! \fn Node* DecisionDiagram::partialAddDecisionDiagram(DecisionDiagram *dd2, *n)
 *  \brief "Partial copy". Copies only the component of the graph reachable from n. This is especially useful if the graph is a tree. The nodes and edges will be added to this decision diagram
 *  i.e. existing element will be kept.
 *  \param dd2 Template decision diagram
 *  \param n Some node of dd2
 *  \return Node* A pointer to the root node of the added decision diagram within this decision diagram.
 *  \throws InvalidDecisionDiagram If dd2 and this decision diagram have node labels in common.
 */

/*! \fn void DecisionDiagram::setRoot(Node* root)
 * Changes the root of this decision diagram.
 *  \param root A pointer to the new root node.
 *  \throws InvalidDecisionDiagram If root is not part of this decision diagram
 */

/*! \fn void DecisionDiagram::useUniqueLabels(DecisionDiagram* dd2)
 * Modifies the labels of this diagram's nodes s.t. they are unique also within dd2.
 *  \param dd2 A reference decision diagram. The node's labels will be uique within this decision diagram and within dd2.
 */

/*! \fn std::string DecisionDiagram::getUniqueLabel(std::string proposal) const
 * Returns a new node label which is as similar to the proposal as possible, such that it is unique within the decision diagram.
 *  \param proposal The desired node label. If it is unique, it is returned unchanged. Otherwise, a number will be added such that it becomes unique.
 *  \param std::string A new unique node label similar or equal to the proposal
 */

/*! \fn std::set<Node*> DecisionDiagram::getNodes() const
 * Returns a set with the nodes (pointers) of this decision diagram.
 *  \return std::set<Node*> Nodes (pointers) of this decision diagram.
 */

/*! \fn std::set<LeafNode*> DecisionDiagram::getLeafNodes() const
 * Returns a set with the leaf nodes (pointers) of this decision diagram.
 *  \return std::set<LeafNode*> Leaf nodes (pointers) of this decision diagram.
 */

/*! \fn std::set<Edge*> DecisionDiagram::getEdges() const
 * Returns a set with the edges (pointers) of this decision diagram.
 *  \return std::set<Edge*> Edges (pointers) of this decision diagram.
 */

/*! \fn Node* DecisionDiagram::getRoot() const
 * Returns a pointer to the root node of this decision diagram.
 *  \return Node* A pointer to the root node of this decision diagram.
 */

/*! \fn Node* DecisionDiagram::getNodeByLabel(std::string label) const
 * Looks up a node in this decision diagram by it's label.
 *  \return Node* A pointer to the node with the given label.
 *  \throw InvalidDecisionDiagram If the decision diagram does not contain a node with the given label.
 */

/*! \fn int DecisionDiagram::nodeCount() const
 * Returns the number of nodes in this decision diagram.
 *  \return int Number of nodes in this decision diagram.
 */

/*! \fn int DecisionDiagram::leafCount() const
 * Returns the number of leaf nodes in this decision diagram.
 *  \return int Number of leaf nodes in this decision diagram.
 */

/*! \fn int DecisionDiagram::edgeCount() const
 * Returns the number of edges in this decision diagram.
 *  \return int Number of edges in this decision diagram.
 */

/*! \fn std::vector<Node*> DecisionDiagram::bool containsCycles() const
 * Checks if the decision diagram contains at least one cycle. While most kinds of inconsistencies are automatically prevented while the decision diagram is creates, cycles are not detected on the fly because of performance reasons. This method starts a cycle detection algorithm and returns the result.
 *  \return std::vector<Node*> Will be empty if the decision diagram contains no cycle. Otherwise, the result will be the list of nodes building a cycle.
 */

/*! \fn bool DecisionDiagram::isTree() const
 * Checks if this decision diagram is a tree.
 *  \throws bool True iff this decision diagram is a tree, otherwise false.
 */

/*! \fn virtual bool DecisionDiagram::operator==(const DecisionDiagram& dd2) const
 * Compares this decision diagram with another one _semantically_. Two decision diagrams are equivalent if they encode the same classifier.
 *  \throws bool True iff the decision diagrams are semantically equivalent.
 */

/*! \fn virtual bool DecisionDiagram::operator!=(const DecisionDiagram& dd2) const
 * Compares this decision diagram with another one _semantically_. Two decision diagrams are equivalent if they encode the same classifier.
 *  \throws bool True iff the decision diagrams are not semantically equivalent.
 */

/*! \fn bool DecisionDiagram::containsPath(const Node* from, const Node* to) const;
 * Checks if there is a path from a certain node to a certain other one
 *  \throws bool True iff there is a path from "from" to "to"
 */

/*! \fn AtomSet DecisionDiagram::toAnswerSet() const
 * Creates an answer set representing this decision diagram with the predicates root(Name), innernode(Label), leafnode(Label, Classification), edge(Node1, Node2, Operand1, comparisonOperator, Operand2) and elseedge(Node1, Node2).
 *  \return AtomSet An answer set representing this decision diagram with the predicates root(Name), innernode(Label), leafnode(Label, Classification), edge(Node1, Node2, Operand1, comparisonOperator, Operand2) and elseedge(Node1, Node2).
 */

/*! \fn std::string DecisionDiagram::toString() const
 * Returns a string representation of this decision diagram.
 *  \return A string representation of this decision diagram.
 */
