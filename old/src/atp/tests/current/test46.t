#
argument {
	At(paul, y);
	forall(x)((At(x, y) && (y != z)) --> ~At(x, z));
	forall(x)(At(x, y) --> ~At(x, z));
	SOS ?- forsome(x)~At(paul, x);
}
#
quit;
#
