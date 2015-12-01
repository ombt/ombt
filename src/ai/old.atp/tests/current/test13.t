#
argument {
	SOS forall(u)(S(u) --> forall(z)(R(z) --> ~P(u, z)));
	SOS forsome(y)(S(y) && forall(w)(Q(w) --> P(y, w)));
	SOS |- forall(y)(Q(y) --> ~R(y));
}
#
quit;
