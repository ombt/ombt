#
argument {
	<x><y><z>((F(x, y) && F(z, x)) --> G(z, y));
	<x>[y]F(y, x);
	SOS |- <x>[y]G(y, x);
}
#
argument {
	<x><y><z>((F(x, y) && F(z, x)) --> G(z, y));
	<x>[y]F(y, x);
	SOS ?- <x>[y]G(y, x);
}
#
quit;
#
