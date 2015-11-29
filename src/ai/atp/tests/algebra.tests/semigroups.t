#
#include "options"
#
argument {
#
# tests for semigroups
#
#include "axioms/semigroups"
#
SOS |- forall(x)m(m(x, x), x) = x;
}
#
argument {
#
# tests for semigroups
#
#include "axioms/semigroups"
#
SOS |- forall(x)m(m(x, x), x) = m(x, x);
}
#
argument {
#
# tests for semigroups
#
#include "axioms/semigroups"
#
SOS |- forall(x)forall(y)m(m(x, y), x) = m(m(x, x), y);
#
}
