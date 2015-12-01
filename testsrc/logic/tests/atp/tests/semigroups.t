argument {
#
# tests for semigroups
#
#include "axioms/semigroups"
#
SOS |- <x>m(m(x, x), x) = x;
#
SOS |- <x>m(m(x, x), x) = m(x, x);
#
SOS |- <x><y>m(m(x, y), x) = m(m(x, x), y);
#
}
