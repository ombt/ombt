#
argument {
	forall(x)forall(y)(F(x, y) --> ~F(y, x));
	SOS |- forall(x)~F(x, x);
}
#
quit;
