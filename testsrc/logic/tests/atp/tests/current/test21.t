#
# schaum's outline page 157, section III problems.
#
argument {
	forall(x)F(x);
	SOS |- F(a) && (F(b) && (F(c) && F(d)));
}
#
argument {
	forall(x)(F(x) || G(x));
	G(a);
	SOS |- G(a);
}
#
argument {
	~F(a);
	SOS |- ~forall(x)(F(x) && G(x));
}
#
argument {
	forall(x)(F(x) <--> R);
	R;
	SOS |- F(a);
}
#
argument {
	forall(x)(~F(x) || ~G(x));
	SOS |- ~(F(a) && G(a));
}
#
argument {
	forall(x)(F(x) --> G(x));
	SOS |- forall(x)(~G(x) --> ~F(x));
}
#
argument {
	forall(x)(F(x) --> G(x));
	SOS |- forall(x)~G(x) --> forall(x)~F(x);
}
#
argument {
	forall(x)forall(y)F(x, y);
	SOS |- forall(x)F(x, x);
}
#
argument {
	forall(x)F(x);
	SOS |- forall(x)G(x) --> forall(x)(F(x) && G(x));
}
#
argument {
	forall(x)forall(y)(F(x, y) --> ~F(y, x));
	SOS |- forall(x)~F(x, x);
}
#
argument {
	forall(x)F(x);
	SOS |- forsome(x)F(x);
}
#
argument {
	~forsome(x)F(x);
	SOS |- ~F(a);
}
#
argument {
	forsome(x) ~F(x);
	SOS |- ~forall(x)F(x);
}
#
argument {
	forall(x)(F(x) --> G(x));
	SOS |- forsome(x)F(x) --> forsome(x)G(x);
}
#
argument {
	~forsome(x)(F(x) && G(x));
	SOS |- forall(x)(~F(x) || ~G(x));
}
#
argument {
	~forall(x)(F(x) && G(x));
	SOS |- forsome(x)(~F(x) || ~G(x));
}
#
argument {
	~forsome(x)forsome(y)L(x, y);
	SOS |- forall(x)~L(x, x);
}
#
argument {
	forsome(x)F(x);
	SOS |- forsome(x)forsome(y)(F(x) && F(y));
}
#
argument {
	forall(x)~F(x);
	SOS |- forall(x)(F(x) --> G(x));
}
#
argument {
	forall(x)~F(x);
	SOS |- forall(x)(F(x) --> ~G(x));
}
#
argument {
	forall(x)forall(y)forall(z)((L(x, y) && L(y, z)) --> ~L(x, z));
	SOS |- forall(x)~L(x, x);
}
#
argument {
	SOS |- ~forsome(x)(F(x) && ~F(x));
}
#
argument {
	SOS |- forsome(x)F(x) || forsome(x)~F(x);
}
#
argument {
	SOS |- forsome(x)F(x) || forall(x)~F(x);
}
#
argument {
	SOS |- ~forsome(x)forall(y)(L(x, y) <--> ~L(x, x));
}
#
argument {
	SOS |- forall(x)forsome(y)(x = y);
}
#
argument {
	SOS |- forall(x)forall(y)((x = y) <--> (y = x));
}
#
argument {
	F(a);
	~F(a);
	SOS |- ~forall(x)forall(y)(x = y);
}
#
argument {
	forall(x)((x = a) || (x = b));
	forsome(x)F(x);
	~F(a);
	SOS |- F(b);
}
#
argument {
	SOS |- forall(x)forall(y)((x=y) --> (F(x) <--> F(y)));
}
#
quit;
