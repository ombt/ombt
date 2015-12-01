#
argument {
	Q(b);
	forall(z)(~P(z, a) || R(c));
	forall(u)forall(w)(P(c, u) || R(w));
	SOS |- forall(x)forall(y)(~Q(x) || ~R(y));
}
#
quit;
