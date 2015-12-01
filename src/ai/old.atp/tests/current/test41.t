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
	SOS ?- forall(x)forall(y)((F(x, y) && (x = y)) --> F(y, x));
}
#
quit;
