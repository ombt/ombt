#
# tests for boolean algebras
#
program {
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
}
