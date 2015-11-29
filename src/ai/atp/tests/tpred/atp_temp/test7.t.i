#
# test equality axioms for functions and predicates
#
argument {
	<x><y>(g(x) = h(x,y));
	P(g(a));
	P(h(a,b)) --> Q(a, b);
	SOS |- Q(a,b);
}
#
quit;
