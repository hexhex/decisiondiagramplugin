[common signature]
predicate: root/1;
predicate: innernode/1;
predicate: leafnode/2;
predicate: conditionaledge/5;
predicate: elseedge/2;

[belief base]
name: tnm;
mapping: "
	root(checkM).

	innernode(checkM).
	innernode(checkT).
	innernode(checkN).

	leafnode(stage1, stage1).
	leafnode(stage2, stage2).
	leafnode(stage3, stage3).
	leafnode(stage4, stage4).

	conditionaledge(checkM, stage4, m, \'=\', \'1\').
	elseedge(checkM, checkN).

	conditionaledge(checkN, stage3, n, \'>\', \'0\').
	elseedge(checkN, checkT).

	conditionaledge(checkT, stage2, t, \'>=\', \'3\').
	elseedge(checkT, stage1).
";

[merging plan]
{

				tnm
}
