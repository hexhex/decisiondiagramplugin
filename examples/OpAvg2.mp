[common signature]
predicate: root/1;
predicate: innernode/1;
predicate: leafnode/2;
predicate: conditionaledge/5;
predicate: elseedge/2;

[belief base]
name: kb1;
mapping: "
	root(a).
	innernode(a).
	innernode(b).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	leafnode(leaf3, class3).
	conditionaledge(a, leaf1, x, \'<\', \'10\').
	elseedge(a, b).
	conditionaledge(b, leaf2, y, \'<\', \'100\').
	elseedge(b, leaf3).
";

[belief base]
name: kb2;
mapping: "
	root(a).
	innernode(a).
	innernode(b).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	leafnode(leaf3, class3).
	conditionaledge(a, leaf1, x, \'<\', \'20\').
	elseedge(a, b).
	conditionaledge(b, leaf2, y, \'<\', \'200\').
	elseedge(b, leaf3).
";

[merging plan]
{
	operator: avg;
	{
		kb1
	};
	{
		kb2
	};
}
