argument {
(A && B) --> (C <--> ~D);
(B && C) --> (A <--> D);
(~A && ~B) <--> (~C && ~D);
SOS |- (~(A && B && C)) && ((~A && ~B) --> ~C);
}
quit;
