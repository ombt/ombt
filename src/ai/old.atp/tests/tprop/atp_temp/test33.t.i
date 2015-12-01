#;
argument {
A --> (B && C);
(B || D) --> E;
D || A;
SOS |- E;
}
#;
argument {
(A && B) --> (C && D);
B --> ~D;
SOS |- ~A || ~B;
}
#;
argument {
(A || B) --> (C && D);
(C || E) --> (D --> ~A);
SOS |- ~A;
}
#;
argument {
W --> (X --> Y);
W --> X;
~Z || (Y || W);
SOS |- ~Z || Y;
}
#;
argument {
(B --> A) --> C;
(A --> B) --> C;
SOS |- C;
}
#;
argument {
~E;
SOS |- ~(((A --> B) &&  (B --> C)) && ((A --> C) --> E));
}
#;
argument {
(~T || U) --> (W && X);
(W || Y) --> (X --> T);
SOS |- T;
}
#;
argument {
(A && B) || C;
~C || B;
SOS |- A --> B;
}
#;
argument {
(A && B) --> (C || D);
(A --> C) --> (B --> D);
B;
SOS |- D;
}
#;
argument {
A --> (~B --> C);
(A --> B) --> C;
SOS |- C;
}
#;
argument {
A <--> (B <--> C);
SOS |- (A <--> B) <--> C;
}
#;
argument {
(A && B) <--> ~C;
A --> B;
SOS |- C <--> ~A;
}
#;
quit;
