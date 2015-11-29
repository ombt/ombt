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
