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
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	conditionaledge(a, leaf1, a, \'<\', \'10\').
	elseedge(a, leaf2).
";

[belief base]
name: kb2;
mapping: "
	root(a).
	innernode(a).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	conditionaledge(a, leaf1, a, \'<\', \'20\').
	elseedge(a, leaf2).
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
