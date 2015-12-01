#
argument {
	forall(x)forall(y)(L(x,y) --> L(y, x));
	forsome(x)L(a, x);
	SOS |- forsome(x)L(x, a);
}
#
quit;.
