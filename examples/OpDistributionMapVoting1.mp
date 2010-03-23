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
	leafnode(leaf1A, \'x {x:5,y:2}\').
	leafnode(leaf2A, \'y {x:1,y:2}\').
	conditionaledge(rootA, leaf1A, a, \'<\', x).
	elseedge(rootA, leaf2A).
";

[belief base]
name: kb2;
mapping: "
	root(rootB).
	innernode(rootB).
	leafnode(leaf1B, \'y {x:1,y:2}\').
	leafnode(leaf2B, \'x {x:2,y:0}\').
	conditionaledge(rootB, leaf1B, b, \'<\', x).
	elseedge(rootB, leaf2B).
";

[merging plan]
{
	operator: distributionmapvoting;
	{
		kb1
	};
	{
		kb2
	};
}
