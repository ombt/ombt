argument {
#
# tests for groups
#
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
# let p be "+"
# let i be "-"
#
############################################################################
#
forall(x)p(x, 0) = x;
forall(x)p(x, i(x)) = 0;
forall(x)forall(y)forall(z)p(p(x, y), z) = p(x, p(y, z));
#

#
SOS |- <x>p(x, x) = 0;
#
# SOS |- <x>p(p(x, x), x) = x;
#
# SOS |- <x><y>p(x, y) = p(y, x);
#
# SOS |- <x><y>p(p(x, y), i(x)) = p(y, y);
#
# SOS |- <x><y>p(p(x, y), p(i(x), i(y))) = 0;
#
}
