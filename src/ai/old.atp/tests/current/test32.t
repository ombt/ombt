#
#include "options"
#
options {
	cconst7 = 0;
}
#
argument {
	SOS |- forall(x)forall(y)((F(x, y) && (x = y)) --> F(y, x));
}
#
options {
	cconst7 = 5;
}
#
argument {
	SOS |- forall(x)forall(y)((F(x, y) && (x = y)) --> F(y, x));
}
#
options {
	cconst7 = 10;
}
#
argument {
	SOS |- forall(x)forall(y)((F(x, y) && (x = y)) --> F(y, x));
}
#
options {
	cconst7 = 20;
}
#
argument {
	SOS |- forall(x)forall(y)((F(x, y) && (x = y)) --> F(y, x));
}
#
quit;
