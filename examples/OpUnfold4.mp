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
	conditionaledge(root, leaf1, x, \'<\', y).
	elseedge(root, leaf1).
";

[merging plan]
{
	operator: unfold;
	{
		kb1
	};
}
