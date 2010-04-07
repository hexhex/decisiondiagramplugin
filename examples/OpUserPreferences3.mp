[common signature]
predicate: root/1;
predicate: innernode/1;
predicate: leafnode/2;
predicate: conditionaledge/5;
predicate: elseedge/2;

[belief base]
name: kb1;
mapping: "
	root(rootA).
	innernode(rootA).
	leafnode(leaf1A, class1).
	leafnode(leaf2A, class2).
	conditionaledge(rootA, leaf1A, a, \'<\', x).
	elseedge(rootA, leaf2A).
";

[belief base]
name: kb2;
mapping: "
	root(rootB).
	innernode(rootB).
	leafnode(leaf1B, class2).
	leafnode(leaf2B, class3).
	conditionaledge(rootB, leaf1B, b, \'<\', x).
	elseedge(rootB, leaf2B).
";

[belief base]
name: kb3;
mapping: "
	root(rootC).
	innernode(rootC).
	innernode(v).
	leafnode(leaf1C, class3).
	leafnode(leaf2C, class1).
	leafnode(leaf3C, class2).
	conditionaledge(rootC, v, c, \'<\', x).
	elseedge(rootC, leaf1C).
	conditionaledge(v, leaf2C, cc, \'<\', x).
	elseedge(v, leaf3C).
";

[merging plan]
{
	operator: userpreferences;
	preferencerule: "class3 >> class1";
	preferencerule: "class3 >> class2";
	preferencerule: "class1 >> class2";
	preferencerule: "class2 >2> class1";
	{
		kb1
	};
	{
		kb2
	};
	{
		kb3
	};
}
