#
#include "options"
#
options {
	gconst1 = 1;
	uconst1 = 1;
	cconst1 = 0;
}
#
argument {
	SOS |- <x><y><z><t>((F(x, y) && (x = z) && (y = t)) --> F(z, t));
}
#
quit;
