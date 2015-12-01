#
# tests for rings
#
program {
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
# let P be "+"
# let M be "*"
# let N be "-"
#
############################################################################
#
P(x, 0) = x;
P(x, N(x)) = 0;
P(x, y) = P(y, x);
P(x, P(y, z)) = P(P(x, y), z);
M(x, 1) = x;
M(1, x) = x;
M(x, M(y, z)) = M(M(x, y), z);
M(x, P(y, z)) = P(M(x, y), M(x, z));
M(P(x, y), z) = P(M(x, z), M(y, z));
#

#
|- M(x, x) = x;
#
|- P(x, x) = 0;
#
|- M(x, y) = M(y, x);
#
}
