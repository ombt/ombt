#
# tests for boolean algebras
#
#include "axioms/options"
#
argument {
#include "axioms/boolean"
SOS |- (<x><y>o(x, n(y)) = n(o(x, y)));
}
#
argument {
#include "axioms/boolean"
SOS |- (<x><y>n(o(x, y)) = o(n(x), n(y)));
}
#
argument {
#include "axioms/boolean"
SOS |- (<x><y><z>o(x, n(a(y, z))) = a(o(x, n(y)), o(x, n(z))));
}
#
quit;
