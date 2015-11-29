#
argument {
	SOS Q(b);
	SOS forall(z)(~P(z, a) || R(c));
	SOS forall(u)forall(w)(P(c, u) || R(w));
	SOS forall(x)forall(y)(~Q(x) || ~R(y));
}
#
quit;
