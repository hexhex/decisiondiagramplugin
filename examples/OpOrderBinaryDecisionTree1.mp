[common signature]
predicate: root/1;
predicate: innernode/1;
predicate: leafnode/2;
predicate: conditionaledge/5;
predicate: elseedge/2;

[belief base]
name: kb1;
mapping: "
	root(b).
	innernode(b).
	innernode(a).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	leafnode(leaf3, class2).
	conditionaledge(b, leaf1, b, \'<\', x).
	elseedge(b, a).
	conditionaledge(a, leaf2, a, \'<\', x).
	elseedge(a, leaf3).
";

[merging plan]
{
	operator: orderbinarydecisiontree;
	{
		kb1
	};
}
