#
#include "options"
#
argument {
#
# tests for rings
#
#include "axioms/rings"
#
SOS |- forall(x)m(x, x) = x;
}
#
argument {
#
# tests for rings
#
#include "axioms/rings"
#
SOS |- forall(x)p(x, x) = 0;
}
#
argument {
#
# tests for rings
#
#include "axioms/rings"
#
SOS |- forall(x)forall(y)m(x, y) = m(y, x);
#
}
