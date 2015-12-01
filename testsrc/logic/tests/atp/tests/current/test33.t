#
argument {
	forall(x)forall(y)((P(x, y) && P(y, z)) --> G(x, z));
	forall(y)forsome(x)P(x, y);
	SOS |- forsome(x)forsome(y)G(x, y);
}
#
quit;
