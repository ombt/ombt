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
SOS |- forall(x)p(x, x) = 0;
}
#
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
SOS |- forall(x)p(p(x, x), x) = x;
}
#
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
SOS |- forall(x)forall(y)p(x, y) = p(y, x);
#
}
#
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
SOS |- forall(x)forall(y)p(p(x, y), i(x)) = p(y, y);
}
#
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
SOS |- forall(x)forall(y)p(p(x, y), p(i(x), i(y))) = 0;
#
}
