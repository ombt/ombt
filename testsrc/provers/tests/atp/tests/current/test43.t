#
#include "options"
#
argument {
	forall(x)(~A(x) || F(x) || G(f(x)));
	forall(x)(~F(x) || B(x));
	forall(x)(~F(x) || C(x));
	forall(x)(~G(x) || B(x));
	forall(x)(~G(x) || D(x));
	forall(x)(A(g(x)) || F(h(x)));
	SOS ?- forsome(x)forsome(y)((B(x) && C(x)) || (D(y) && B(y)));
}
#
argument {
	forall(x)(~A(x) || F(x) || G(f(x)));
	forall(x)(~F(x) || B(x));
	forall(x)(~F(x) || C(x));
	forall(x)(~G(x) || B(x));
	forall(x)(~G(x) || D(x));
	forall(x)(A(g(x)) || F(h(x)));
	SOS |- forsome(x)forsome(y)((B(x) && C(x)) || (D(y) && B(y)));
}
#
quit;
#
