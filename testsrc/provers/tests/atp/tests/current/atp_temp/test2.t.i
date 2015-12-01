#
argument {
A(f(a));
<x>((A(f(x)) || A(f(a))) --> B(g(x)));
<y>(A(f(y)) --> B(g(y)));
SOS |- A(f(a)) --> B(g(a));
}
#
argument {
A(f(a));
forall(x)((A(f(x)) || A(f(a))) --> B(g(x)));
forall(y)(A(f(y)) --> B(g(y)));
SOS |- A(f(a)) --> B(g(a));
}
#
quit;
