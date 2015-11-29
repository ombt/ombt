#
# test for determining connections between cities.
#
# D(a, b) - means a direct flight from city A to B.
# 
# rule for determining connections.
#
# forall(x)forall(y)formsome(z)(D(x, z) && C(z, y) --> C(x, y))
# forall(x)forall(y)(d(x, y) <--> d(y, x))
#
# let the cities be:
#
# seattle, nycity, la, boston, miami, chicago, neworleans, montreal
#
options {
	gconst1 = 1;
	uconst1 = 1;
	cconst1 = 0;
}
#
argument {
	D(seattle, chicago);
	D(chicago, neworleans);
	forall(x)forall(y)(D(x, y) <--> D(y, x));
	forall(x)forall(y)(D(x, y) --> C(x, y));
	forsome(x)forsome(y)forsome(z)((D(x, z) && C(z, y)) --> C(x, y));
	SOS ?- [x](C(seattle,x)&&C(x,neworleans));
}
#
quit;
#
