[common signature]
predicate: root/1;
predicate: innernode/1;
predicate: leafnode/2;
predicate: conditionaledge/5;
predicate: elseedge/2;

[belief base]
name: kb1;
mapping: "
	root(root).
	innernode(root).
	innernode(v1).
	innernode(v2).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	leafnode(leaf3, class2).
	leafnode(leaf4, class3).
	conditionaledge(root, v1, v, \'<\', y).
	elseedge(root, v2).
	conditionaledge(v1, leaf1, x, \'<\', a).
	elseedge(v1, leaf2, y, \'<\', b).
	conditionaledge(v2, leaf3, x, \'<\', a).
	elseedge(v2, leaf4, y, \'<\', b).
";

[merging plan]
{
	operator: simplify;
	{
		kb1
	};
}
