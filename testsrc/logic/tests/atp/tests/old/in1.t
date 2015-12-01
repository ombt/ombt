#
# problem 18, page 185 in Deduction by D. Bonevac
#
argument {
<x>([y](A(x) && B(x, y)) --> C(x));
[y](D(y) && [x]((F(x) && G(x)) && B(y, x)));
<x>(G(x) --> A(x));
SOS |- [x](C(x) && D(x));
}
#
quit;

