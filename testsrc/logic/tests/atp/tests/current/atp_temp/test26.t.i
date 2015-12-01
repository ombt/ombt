#
argument {
	forall(x)(L(x) --> D(x));
	forall(x)((D(x) && I(x)) --> S(x));
	L(c) && I(c);
	SOS |- S(c);
}
#
argument {
	SOS |- forall(x)(A(x) --> B(x)) --> forsome(x)(A(x) --> forsome(x)B(x));
}
#
quit;
