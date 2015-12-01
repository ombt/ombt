#
argument {
	forall(u)(S(u) --> forall(z)(R(z) --> ~P(u, z)));
	forsome(y)(S(y) && forall(w)(Q(w) --> P(y, w)));
	SOS |- forall(y)(Q(y) --> ~R(y));
}
#
quit;
