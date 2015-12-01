#
argument {
	<x>(P(x) --> ~M(x));
	P(john);
	SOS |- M(john);
}
#
argument {
	<x>(P(x) --> ~M(x));
	P(john);
	SOS |- ~M(john);
}
#
argument {
	<x>(P(x) --> ~M(x));
	P(john);
	SOS ?- [x]M(x);
}
#
argument {
	<x>(P(x) --> ~M(x));
	P(john);
	SOS ?- [x]~M(x);
}
#
quit;
