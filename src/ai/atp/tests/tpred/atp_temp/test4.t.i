#
# test equality axioms for functions
#
argument {
	a = a;
	|- a = a;
}
#
argument {
	f(a) = f(a);
	|- f(a) = f(a);
}
#
argument {
	g(a,b,c) = f(a,b,c,d);
	|- f(a,b,c,d) = f(a,b,c,d);
}
#
argument {
	<x>(x = x);
	|- [x](x = x);
}
#
argument {
	<x>(f(x) = f(x));
	|- [x](f(x) = f(x));
}
#
argument {
	<x><y><z><t>(g(x,y,z) = f(x,y,z,t));
	|- <t>(g(a,b,c) = f(a,b,c,t));
}
#
quit;
