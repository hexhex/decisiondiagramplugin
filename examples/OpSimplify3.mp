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
	innernode(x).
	innernode(y).
	leafnode(leaf11, class1).
	leafnode(leaf12, class2).
	leafnode(leaf21, class1).
	leafnode(leaf22, class2).
	leafnode(leaf3, commonclass).
	leafnode(leaf4, commonclass).
	conditionaledge(root, v1, v, \'<\', y).
	conditionaledge(root, v2, w, \'<\', y).
	conditionaledge(v1, x, x, \'<\', a).
	elseedge(v1, y).
	conditionaledge(x, leaf11, xx, \'<\', c).
	elseedge(x, leaf12).
	conditionaledge(y, leaf21, xx, \'<\', c).
	elseedge(y, leaf22).
	conditionaledge(v2, leaf3, something, \'<\', e).
	elseedge(v2, leaf4).
";

[merging plan]
{
	operator: simplify;
	{
		kb1
	};
}
