#
argument {
	At(paul, madrid);
	<x><y><z>(At(x, y) --> ~At(x, z));
	SOS ?- forsome(x)~At(paul, x);
}
#
quit;
#
