# basic properties for groups.
# let P(x, y, z) --> x * y = z, where x, y, z are elements of a group, and
# "*" is the group binary operation. Also, "e" is the group identity.
#
#############################################################################
#
argument {
#
# group axiom 1
#
# x, y in G, then x*y in G (closure property)
#
<x><y>[z]P(x, y, z);
#
# group axiom 2
#
# x, y, z, in G, then x*(y*x) = (x*y)*z (associative property)
#
<x><y><z><u><v><w>(P(x, y, u) && P(x, z, v) && P(u, z, w) --> P(x, v, w)) && <x><y><z><u><v><w>(P(x, y, u) && P(y, z, v) && P(x, v, w) --> P(u, z, w));
#
# group axiom 3
#
# x*e = e*x = x for all x in G (identity property)
#
<x>P(e, x, x) && <x>P(e, x, x);
#
# group axiom 4
#
# for every x in G, there exists an element ix in G such that,
# ix*x = x*ix = e (inverse property)
#
<x>P(x, i(x), e) && <x>P(i(x), x, e);
#
# conclusion to br proved
#
# if x*x = e for all x in G, then G is commutative, that is, 
# u*v = v*u for all u, v in G.
#
|- <x>P(x, x, e) --> <u><v><w>(P(u, v, w) --> P(v, u, w));
}
#
quit;
