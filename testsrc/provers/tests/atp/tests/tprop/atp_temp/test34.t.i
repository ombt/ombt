#;
# ALL THESE TESTS ARE INVALID.;
#;
argument {
(S --> T) && (T --> S);
(U && T) || (~T && ~U);
(U || V) || (S || T);
~U --> (W && X);
(V --> ~S) && (~V --> ~Y);
X --> (~Y --> ~X);
(U || S) && (V || Z);
SOS |- X && Z;
}
#;
quit;
