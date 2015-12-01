#
argument {
	forsome(x)forsome(y)L(x, y);
	SOS |- forsome(x)forsome(y)(L(x, y) && ~(x = y));
}
#
quit;
