#
# tests for boolean algebras
#
#
#
# axioms for boolean algebras
#
# algebras are defined using the following language:
#
# L(B) = { v, ^, ', 0, 1 } where v, ^,  are binary, ' is unary,
# and 0, 1 are constants,
#
# the axioms for boolean algebras are then:
#
# x v y = y v x
# x ^ y = y ^ x
# x v (y v z) = (x v y) v z
# x ^ (y ^ z) = (x ^ y) ^ z
# x ^ (x v y) = x
# x v (x ^ y) = x
# x ^ (y v z) = (x ^ y) v (x ^ z)
# x v x' = 1
# x ^ x' = 0
# x v 1 = 1
# x ^ 0 = 0
# 
# let A be "^"
# let O be "v"
# let N be "'"
#
############################################################################
#
O(x, y) = O(y, x);
A(x, y) = A(y, x);
O(x, O(y, z)) = O(O(x, y), z);
A(x, A(y, z)) = A(A(x, y), z);
A(x, O(x, y)) = x;
O(x, A(x, y)) = x;
A(x, O(y, z)) = O(A(x, y), A(x, z));
O(x, N(z)) = 1;
A(x, N(z)) = 0;
O(x, 1) = 1;
A(x, 0) = 0;
#

#
|- O(x, N(y)) = N(O(x, y));
#
|- N(O(x, y)) = O(N(x), N(y));
#
|- O(x, A(N(y, z))) = A(O(x, N(y)), O(x, N(z)));
#
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
#
# tests for semigroups
#
#
#
# axioms for semigroups
#
# semigroups are defined using the following language:
#
# L(SG) = { * } where * is binary.
#
# the axioms for semigroups are then:
#
# (x * y) * z = x * (y * z)
#
# let M be "*"
#
############################################################################
#
M(M(x, y), z) = M(x, M(y, z));
#

#
|- M(M(x, x), x) = x;
#
|- M(M(x, x), x) = M(x, x);
#
|- M(M(x, y), x) = M(M(x, x), y);
#
