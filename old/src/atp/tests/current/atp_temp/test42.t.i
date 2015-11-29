#
argument {
	forall(x)forall(y)((P(x,y) && P(y,z)) --> G(x,z));
	forall(x)forsome(y)P(x,y);
	SOS ?- forsome(x)forsome(y)G(x,y);
}
#
quit;
