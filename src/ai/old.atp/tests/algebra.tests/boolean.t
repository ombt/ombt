#
#include "options"
#
argument {
#
# tests for boolean algebras
#
#include "axioms/boolean"
#
SOS |- forall(x)forall(y)o(x, n(y)) = n(o(x, y));
}
#
argument {
#
# tests for boolean algebras
#
#include "axioms/boolean"
#
SOS |- forall(x)forall(y)n(o(x, y)) = o(n(x), n(y));
}
#
argument {
#
# tests for boolean algebras
#
#include "axioms/boolean"
#
SOS |- forall(x)forall(y)forall(z)o(x, n(a(y, z))) = a(o(x, n(y)), o(x, n(z)));
}
