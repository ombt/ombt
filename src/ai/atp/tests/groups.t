argument {
#
# tests for groups
#
#include "axioms/groups"
#
SOS |- <x>p(x, x) = 0;
#
# SOS |- <x>p(p(x, x), x) = x;
#
# SOS |- <x><y>p(x, y) = p(y, x);
#
# SOS |- <x><y>p(p(x, y), i(x)) = p(y, y);
#
# SOS |- <x><y>p(p(x, y), p(i(x), i(y))) = 0;
#
}
