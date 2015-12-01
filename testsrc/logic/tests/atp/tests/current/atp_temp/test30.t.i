argument {
	SOS forall(x)forall(y)F(x, y);
	SOS forall(x)forall(y)(G(x, y) || ~F(y, p(x, y)) || ~F(p(x, y), p(x, y)));
	SOS forall(x)forall(y)(~F(y, p(x, y)) || ~F(p(x, y), p(x, y)) || ~G(x, p(x, y)) || ~G(p(x, y), p(x, y)));
}
#
quit;
