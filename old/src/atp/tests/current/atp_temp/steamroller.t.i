#
# schubert's steamroller problem !!!
#
options {
	gconst1 = 0;
	uconst1 = 1;
	cconst7 = 1;
}
#
argument {
[x1][x2][x3][x4][x5][x6](W(x1) && F(x2) && B(x3) && C(x4) && S(x5) && G(x6));
<x>((W(x) || F(x) || B(x) || C(x) || S(x)) --> A(x));
<x>(G(x) --> P(x));
<x>(A(x) --> (<y>(P(y) --> EATS(x, y)) || <y>((A(y) && MST(y, x) && ([z](P(z) &&EATS(y, z)))) --> EATS(x, y))));
<x><y>((C(x)&&B(y)) --> MST(x, y));
<x><y>((S(x)&&B(y)) --> MST(x, y));
<x><y>((B(x)&&F(y)) --> MST(x, y));
<x><y>((F(x)&&W(y)) --> MST(x, y));
<x><y>((F(x)&&W(y)) --> ~EATS(y, x));
<x><y>((G(x)&&W(y)) --> ~EATS(y, x));
<x><y>((B(x)&&C(y)) --> EATS(x, y));
<x><y>((B(x)&&S(y)) --> ~EATS(x, y));
<x>(S(x) --> ([y](P(y)&&EATS(x, y))));
<x>(C(x) --> ([y](P(y)&&EATS(x, y))));
#
SOS |- [x][y](A(x)&&A(y)&&(<z>(G(z)-->(EATS(x, y) && EATS(y, z)))));
}
#
quit;
