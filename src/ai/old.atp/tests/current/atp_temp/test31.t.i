#
#
#
# default values for heuristic options.
#
options {
	# gvalue constants - distance from a start node
	gconst1 = 1;
	gconst2 = 0;
	gconst3 = 0;

	# hvalue constants - unit-preference estimate
	uconst1 = 1;
	uconst2 = 0;
	uconst3 = 0;

	# hvalue constants - complexity estimate
	cconst1 = 0;
	cconst2 = 0;
	cconst3 = 0;
	cconst4 = 0;
	cconst5 = 0;
	cconst6 = 0;
	cconst7 = 0;
}
#
#
options {
	cconst7 = 10;
}
#
argument {
	SOS L(1, a);
	SOS <x>(~P(x) || ~D(x, a));
	SOS <x>(~L(1, x) || ~L(x, a) || P(k(x)));
	SOS <x>(~L(1, x) || ~L(x, a) || D(k(x), x));
	SOS <x>D(x, x);
	SOS <x>(P(x) || L(1, q(x)));
	SOS <x>(P(x) || L(q(x), x));
	SOS <x>(P(x) || D(q(x), x));
	SOS |- <x><y><z>(~D(x, y) || ~D(y, z) || D(x, z));
}
#
quit;
