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
	innernode(v3).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	conditionaledge(root, v1, x, \'<\', y).
	conditionaledge(root, v3, xx, \'<\', y).
	elseedge(root, v2).
	conditionaledge(v1, leaf1, z, \'<\', w).
	elseedge(v1, leaf2).
	conditionaledge(v2, leaf1, z, \'<\', w).
	elseedge(v2, leaf2).
	conditionaledge(v3, leaf1, zz, \'<\', w).
	elseedge(v3, leaf2).
";

[merging plan]
{
	operator: unfold;
	{
		kb1
	};
}
