argument {
	forall(x)A(x);
	forall(x)B(x);
	forall(x)(~(~A(x) || ~B(x)) --> C(a));
	SOS |- forsome(x)C(x);
}
#
quit;
