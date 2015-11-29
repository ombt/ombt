argument {
D || [x]C(x);
<x>F(x, a);
<x>F(x, b(a));
SOS |- ~[x]G(x) <--> ~([x](F(x, a) && G(x)) && <y>(G(y) --> F(y, a)));
}
#
quit;
