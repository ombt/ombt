#
# schaum's outline problems
#
argument {
<x>(~F(x) || G(x));
|- ~(F(a) && G(a));
}
#
argument {
<x><y>(F(x, y) --> ~F(y, x));
|- <x>(~F(x, x));
}
#
argument {
~[x](F(x) && G(x));
|-<x>(~F(x) || ~G(x));
}
#
argument {
<x>(~F(x));
|-<x>(F(x) --> ~G(x));
}
#
argument {
<x><y>(<z>((L(x, y) && L(y, z)) --> ~L(x, z)));
|- <x>(~L(x, x));
}
#
argument {
|- ~[x]<y>(L(x, y) <--> ~L(x, x));
}
#
quit;
