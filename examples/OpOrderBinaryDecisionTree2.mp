[common signature]
predicate: root/1;
predicate: innernode/1;
predicate: leafnode/2;
predicate: conditionaledge/5;
predicate: elseedge/2;

[belief base]
name: kb1;
mapping: "
	root(c).
	innernode(b).
	innernode(a).
	innernode(c).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	leafnode(leaf3, class2).
	leafnode(leaf4, class3).
	conditionaledge(c, leaf1, c, \'<\', x).
	elseedge(c, b).
	conditionaledge(b, leaf2, b, \'<\', x).
	elseedge(b, a).
	conditionaledge(a, leaf3, a, \'<\', x).
	elseedge(a, leaf4).
";

[merging plan]
{
	operator: orderbinarydecisiontree;
	{
		kb1
	};
}
