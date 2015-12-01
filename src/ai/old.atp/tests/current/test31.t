#
#include "options"
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
