#
argument {
	At(paul, madrid);
	<x>((x != madrid) --> ~At(paul, x));
	SOS ?- forsome(x)At(paul, x);
}
#
quit;
#
