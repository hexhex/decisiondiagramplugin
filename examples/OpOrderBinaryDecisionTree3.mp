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
	innernode(c).
	innernode(b).
	innernode(a).
	leafnode(leaf1, class1).
	leafnode(leaf2, class2).
	conditionaledge(c, a, c, \'<\', x).
	elseedge(c, b).
	conditionaledge(b, leaf2, b, \'<\', x).
	elseedge(b, leaf1).
	conditionaledge(a, leaf1, a, \'<\', x).
	elseedge(a, leaf2).
";

[merging plan]
{
	operator: orderbinarydecisiontree;
	{
		operator: tobinarydecisiontree;
		{
			operator: unfold;
			{
				kb1
			};
		};
	};
}
