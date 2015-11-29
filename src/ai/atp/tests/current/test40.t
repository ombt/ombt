#
options {
	gconst1 = 1;
	uconst1 = 1;
}
#
argument {
	#
	#include "axioms/groups"
	#
	#include "axioms/group3p"
	#
	SOS ?- forall(x)(p(x,x) = p(1, x));
}
#
quit;
