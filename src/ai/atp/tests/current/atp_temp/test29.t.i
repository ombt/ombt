#
argument {
	forall(x)(F(x) || G(x) || H(x) || I(x));
	forall(x)I(x);
	SOS |- forall(x)I(x);
}
#
quit;
#
