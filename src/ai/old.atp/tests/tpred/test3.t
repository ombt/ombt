#
# test individual operations
#
argument {
	A --> B;
	B;
	|- ~A;
}
#
argument {
	A <--> B;
	B && C;
	B || C;
	B || C;
	|- ~A;
}
#
argument {
	~([x]P(f(x)) --> <y>Q(f(y)));
	|- Q(f(a));
}
#
argument {
	[x]f(x) = f(a);
	~([x]g(x) = g(a));
	[x]g(x) != g(a);
	|- <y>Q(f(y));
}
#
quit;
