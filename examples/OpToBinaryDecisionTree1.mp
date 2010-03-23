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
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	leafnode(leaf3, class3).
	conditionaledge(root, leaf1, x, \'<\', y).
	conditionaledge(root, leaf2, z, \'<\', y).
	elseedge(root, leaf3).
";

[merging plan]
{
	operator: tobinarydecisiontree;
	{
		kb1
	};
}
