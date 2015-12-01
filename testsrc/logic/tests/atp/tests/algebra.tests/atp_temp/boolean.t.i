#
#
#
# default values for heuristic options.
#
options {
	# gvalue constants - distance from a start node
	gconst1 = 1;
	gconst2 = 0;
	gconst3 = 0;

	# hvalue constants - unit-preference estimate
	uconst1 = 1;
	uconst2 = 0;
	uconst3 = 0;

	# hvalue constants - complexity estimate
	cconst1 = 0;
	cconst2 = 0;
	cconst3 = 0;
	cconst4 = 0;
	cconst5 = 0;
	cconst6 = 0;
	cconst7 = 0;
}
#
#
argument {
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
# let a be "^"
# let o be "v"
# let n be "'"
#
############################################################################
#
forall(x)forall(y)o(x, y) = o(y, x);
forall(x)forall(y)a(x, y) = a(y, x);
forall(x)forall(y)forall(z)o(x, o(y, z)) = o(o(x, y), z);
forall(x)forall(y)forall(z)a(x, a(y, z)) = a(a(x, y), z);
forall(x)forall(y)a(x, o(x, y)) = x;
forall(x)forall(y)o(x, a(x, y)) = x;
forall(x)forall(y)forall(z)a(x, o(y, z)) = o(a(x, y), a(x, z));
forall(x)forall(z)o(x, n(z)) = 1;
forall(x)forall(z)a(x, n(z)) = 0;
forall(x)o(x, 1) = 1;
forall(x)a(x, 0) = 0;
#

#
SOS |- forall(x)forall(y)o(x, n(y)) = n(o(x, y));
}
#
argument {
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
# let a be "^"
# let o be "v"
# let n be "'"
#
############################################################################
#
forall(x)forall(y)o(x, y) = o(y, x);
forall(x)forall(y)a(x, y) = a(y, x);
forall(x)forall(y)forall(z)o(x, o(y, z)) = o(o(x, y), z);
forall(x)forall(y)forall(z)a(x, a(y, z)) = a(a(x, y), z);
forall(x)forall(y)a(x, o(x, y)) = x;
forall(x)forall(y)o(x, a(x, y)) = x;
forall(x)forall(y)forall(z)a(x, o(y, z)) = o(a(x, y), a(x, z));
forall(x)forall(z)o(x, n(z)) = 1;
forall(x)forall(z)a(x, n(z)) = 0;
forall(x)o(x, 1) = 1;
forall(x)a(x, 0) = 0;
#

#
SOS |- forall(x)forall(y)n(o(x, y)) = o(n(x), n(y));
}
#
argument {
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
# let a be "^"
# let o be "v"
# let n be "'"
#
############################################################################
#
forall(x)forall(y)o(x, y) = o(y, x);
forall(x)forall(y)a(x, y) = a(y, x);
forall(x)forall(y)forall(z)o(x, o(y, z)) = o(o(x, y), z);
forall(x)forall(y)forall(z)a(x, a(y, z)) = a(a(x, y), z);
forall(x)forall(y)a(x, o(x, y)) = x;
forall(x)forall(y)o(x, a(x, y)) = x;
forall(x)forall(y)forall(z)a(x, o(y, z)) = o(a(x, y), a(x, z));
forall(x)forall(z)o(x, n(z)) = 1;
forall(x)forall(z)a(x, n(z)) = 0;
forall(x)o(x, 1) = 1;
forall(x)a(x, 0) = 0;
#

#
SOS |- forall(x)forall(y)forall(z)o(x, n(a(y, z))) = a(o(x, n(y)), o(x, n(z)));
}