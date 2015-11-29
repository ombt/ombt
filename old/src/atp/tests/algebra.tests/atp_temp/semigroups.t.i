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
# let m be "*"
#
############################################################################
#
forall(x)forall(y)forall(z)m(m(x, y), z) = m(x, m(y, z));
#

#
SOS |- forall(x)m(m(x, x), x) = x;
}
#
argument {
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
# let m be "*"
#
############################################################################
#
forall(x)forall(y)forall(z)m(m(x, y), z) = m(x, m(y, z));
#

#
SOS |- forall(x)m(m(x, x), x) = m(x, x);
}
#
argument {
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
# let m be "*"
#
############################################################################
#
forall(x)forall(y)forall(z)m(m(x, y), z) = m(x, m(y, z));
#

#
SOS |- forall(x)forall(y)m(m(x, y), x) = m(m(x, x), y);
#
}
