#
argument {
	P(j, m);
	SOS |- forsome(x)P(x, m);
}
#
argument {
	P(j, m);
	SOS ?- forsome(x)P(x, m);
}
#
quit;
