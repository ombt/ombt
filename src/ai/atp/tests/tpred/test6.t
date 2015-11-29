#
# test equality axioms for functions and predicates
#
options {
	gconst1 = 0;
}
#
argument {
 	f(a) = g(b);
 	P(f(a));
 	P(g(b)) --> Q(a, b);
 	SOS |- Q(a,b);
}
#
argument {
	f(a) = g(b);
	g(b) = h(a,b);
	P(f(a));
	P(h(a,b)) --> Q(a, b);
	SOS |- Q(a,b);
}
#
argument {
	<x><y>(g(x) = h(x,y));
 	P(f(a));
 	P(h(a,b)) --> Q(a, b);
 	SOS |- Q(a,b);
}
#
quit;
