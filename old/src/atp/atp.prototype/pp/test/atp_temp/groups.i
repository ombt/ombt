#
# tests for groups
#
program
{
#
#
# axioms for groups
#
# groups are defined using the following language:
#
# L(G) = { +, -, 0 } where + is binary, - is unary, and 0 is a constant.
#
# the axioms for rings are then:
#
# x + 0 = x
# x + (-x) = 0
# (x + y) + z = x + (y + z)
# 
# let P be "+"
# let I be "-"
#
############################################################################
#
P(x, 0) = x;
P(x, I(x)) = 0;
P(P(x, y), z) = P(x, P(y, z));
#
	#
	|- P(x, x) = 0;
	#
	|- P(P(x, x), x) = x;
	#
	|- P(x, y) = P(y, x);
	#
	|- P(P(x, y), I(x)) = P(y, y);
	#
	|- P(P(x, y), P(I(x), I(y))) = 0;
	#
}
