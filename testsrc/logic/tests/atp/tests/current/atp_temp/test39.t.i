#
options {
	gconst1 = 1;
	uconst1 = 1;
}
#
argument {
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
#
#
# definitions for a 3x3 group.
#
p(0,0) = 0;
p(0,b) = b;
p(0,c) = c;
p(b,0) = b;
p(b,b) = c;
p(b,c) = 0;
p(c,0) = c;
p(c,b) = 0;
p(c,c) = b;
#
m(0) = 0;
m(b) = c;
m(c) = b;
	#
	SOS |- forall(x)(p(x,x) = p(1, x));
}
#
quit;
