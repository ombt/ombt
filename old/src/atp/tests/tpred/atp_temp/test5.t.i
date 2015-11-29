#
# test equality axioms for functions
#
argument {
	a = b;
	P(a);
	P(b) --> Q(a, b);
	|- Q(a,b);
}
#
argument {
	P(b);
	<x>(P(x) --> Q(a, x));
	|- Q(a,b);
}
#
argument {
	a = b;
	<x><y><z>(P(a,x,y,z) --> P(b,x,y,z));
	|- <x><y><z>P(b,x,y,z);
}
#
argument {
	a = b;
	<x><y><z>(P(a,x,y,z) --> P(b,x,y,z));
	<x><y><z>(P(b,x,y,z) --> Q(x,y,z));
	|- <x><y><z>Q(x,y,z);
}
#
quit;
