#
argument {
	forsome(y)(P(y) && R(y));
	forall(z)(P(z) --> (Q(z) && S(z)));
	SOS |- forsome(x)(R(x) && S(x));
}
#
quit;
