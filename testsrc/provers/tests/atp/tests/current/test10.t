#
argument {
	SOS |- <x><y>((x = y) --> (y = x));
}
#
argument {
	SOS |- <x><y><z>(((x = y) && (y = z)) --> (x = z));
}
#
quit;
