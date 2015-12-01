#
argument {
#
# tests for boolean algebras
#
#include "axioms/boolean"
#
SOS |- (<x><y>o(x, n(y)) = n(o(x, y)));
#
SOS |- (<x><y>n(o(x, y)) = o(n(x), n(y)));
#
SOS |- (<x><y><z>o(x, n(a(y, z))) = a(o(x, n(y)), o(x, n(z))));
#
}
quit;
