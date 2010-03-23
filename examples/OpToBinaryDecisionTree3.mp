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
	leafnode(leaf3, class3).
	conditionaledge(root, v1, x, \'<\', y).
	elseedge(root, v2, z, \'<\', y).
	conditionaledge(v1, leaf1, a, \'<\', y).
	conditionaledge(v1, leaf2, b, \'<\', y).
	elseedge(v1, leaf3).
	conditionaledge(v2, leaf1, aa, \'<\', y).
	conditionaledge(v2, leaf2, bb, \'<\', y).
	elseedge(v2, leaf3).
";

[merging plan]
{
	operator: tobinarydecisiontree;
	{
		operator: unfold;
		{
			kb1
		};
	};
}
