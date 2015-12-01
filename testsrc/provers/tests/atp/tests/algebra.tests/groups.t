#
#include "options"
#
argument {
#
# tests for groups
#
#include "axioms/groups"
#
SOS |- forall(x)p(x, x) = 0;
}
#
argument {
#
# tests for groups
#
#include "axioms/groups"
#
SOS |- forall(x)p(p(x, x), x) = x;
}
#
argument {
#
# tests for groups
#
#include "axioms/groups"
#
SOS |- forall(x)forall(y)p(x, y) = p(y, x);
#
}
#
argument {
#
# tests for groups
#
#include "axioms/groups"
#
SOS |- forall(x)forall(y)p(p(x, y), i(x)) = p(y, y);
}
#
argument {
#
# tests for groups
#
#include "axioms/groups"
#
SOS |- forall(x)forall(y)p(p(x, y), p(i(x), i(y))) = 0;
#
}
