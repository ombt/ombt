argument {
#
# tests for rings
#
#include "axioms/rings"
#
# SOS ?- <x>m(x, x) = x;
# SOS |- <x>m(x, x) = x;
#
# SOS ?- <x>p(x, x) = 0;
# SOS |- <x>p(x, x) = 0;
#
SOS ?- <x><y>m(x, y) = m(y, x);
# SOS |- <x><y>m(x, y) = m(y, x);
#
}
