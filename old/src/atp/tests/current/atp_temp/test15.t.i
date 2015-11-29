argument {
	forall(x)A(x) || forall(x)A(x);
	forall(x)A(x) --> forsome(y)B(y);
	SOS |- forsome(y)B(y);
}
#
quit;
