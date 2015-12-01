#
#include "options"
#
options {
	cconst7 = 10;
}
#
argument {
	L(1, a);
	<x>(~P(x) || ~D(x, a));
	<x>(~L(1, x) || ~L(x, a) || P(k(x)));
	<x>(~L(1, x) || ~L(x, a) || D(k(x), x));
	<x>D(x, x);
	<x>(P(x) || L(1, q(x)));
	<x>(P(x) || L(q(x), x));
	<x>(P(x) || D(q(x), x));
	SOS |- <x><y><z>(~D(x, y) || ~D(y, z) || D(x, z));
}
#
quit;
