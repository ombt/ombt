#
# tests that symbols, etc. from one argument are cleared before the
# the second argument is scanned.
#
argument {
F(x);
SOS |- [x]F(x);
}
#
argument {
<x>F(x);
SOS |- [x]F(x);
}
#
quit;
