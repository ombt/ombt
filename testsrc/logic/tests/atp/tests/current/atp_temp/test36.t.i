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
argument {
	D(seattle, chicago);
	D(seattle, la);
	D(la, chicago);
	D(la, neworleans);
	D(neworleans, chicago);
	D(neworleans, ny);
	D(neworleans, miami);
	D(chicago, ny);
	D(chicago, miami);
	D(boston, ny);
	D(montreal, boston);
	D(montreal, ny);
	#
	forall(x)forall(y)(D(x, y) <--> D(y, x));
	forall(x)forall(y)(D(x, y) --> C(x, y));
	forall(x)forall(y)forall(z)((D(x, z) && C(z, y)) --> C(x, y));
	SOS |- C(seattle, miami) && C(ny, neworleans);
}
#
quit;
