argument {
#
# tests for rings
#
#
#
# axioms for rings
#
# rings are defined using the following language:
#
# L(R) = { +, *, -, 0, 1 } where +, * are binary, - is unary,
# and 0, 1 are constants,
#
# the axioms for rings are then:
#
# x + 0 = x
# x + (-x) = 0
# x + y = y + x
# x + (y + z) = (x + y) + z
# x * 1 = x
# 1 * x = x
# x * (y * z) = (x * y) * z
# x * (y + z) = (x * y) + (x * z)
# (x + y) * z = (x * z) + (y * z)
# 
# let p be "+"
# let m be "*"
# let n be "-"
#
############################################################################
#
forall(x)p(x, 0) = x;
forall(x)p(x, n(x)) = 0;
forall(x)forall(y)p(x, y) = p(y, x);
forall(x)forall(y)forall(z)p(x, p(y, z)) = p(p(x, y), z);
forall(x)m(x, 1) = x;
forall(x)m(1, x) = x;
forall(x)forall(y)forall(z)m(x, m(y, z)) = m(m(x, y), z);
forall(x)forall(y)forall(z)m(x, p(y, z)) = p(m(x, y), m(x, z));
forall(x)forall(y)forall(z)m(p(x, y), z) = p(m(x, z), m(y, z));
#

#
# SOS ?- <x>m(x, x) = x;
# SOS |- <x>m(x, x) = x;
#
# SOS ?- <x>p(x, x) = 0;
# SOS |- <x>p(x, x) = 0;
#
SOS ?- <x><y>m(x, y) = m(y, x);
# SOS |- <x><y>m(x, y) = m(y, x);
#
}
