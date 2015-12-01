#;
# SOLVED PROBLEMS IN CHAPTER 3, PROPOSITIONAL CALCULUS, IN SCHAUM'S.;
#;
argument {
~P --> (Q --> R), ~P, Q;
SOS |- R;
}
#;
argument {
~P --> ~~Q, ~~~P;
SOS |- Q;
}
#;
argument {
P --> (Q && R), P;
SOS |- P && Q;
}
#;
argument {
P && Q;
SOS |- Q && P;
}
#;
argument {
(P && Q) --> ( R && S), ~~P, Q;
SOS |- S;
}
#;
argument {
P;
SOS |- P && P;
}
#;
argument {
P;
SOS |- (P || Q) && (P || R);
}
#;
argument {
P;
~~(P --> Q);
SOS |- (R && S) || Q;
}
#;
argument {
P;
SOS |- P || P;
}
#;
argument {
(P || Q) && (P || R), P --> S, Q --> S, P --> T, R --> T;
SOS |- S && T;
}
#;
argument {
P || P, P --> (Q && R);
SOS |- R;
}
#;
argument {
W <--> (S1 || S2), S2;
SOS |- W;
}
#;
argument {
P --> Q;
(P --> Q) --> (Q --> P);
SOS |- (P <--> Q);
}
#;
argument {
P <--> Q;
SOS |- Q <--> P;
}
#;
# SUPPLEMENTARY PROBLEMS V IN SCHAUMS, PAGE 70.;
#;
argument {
P <--> Q, Q <--> R;
SOS |- P <--> R;
}
#;
argument {
P <--> Q;
SOS |- ~P <--> ~Q;
}
#;
argument {
~P || Q;
SOS |- ~(P && ~Q);
}
#;
argument {
P --> Q, P --> ~Q;
SOS |- ~P;
}
#;
argument {
(P --> Q) && (P --> R);
SOS |- P --> (Q && R);
}
#;
argument {
P --> Q;
SOS |- (P && R) --> (Q && R);
}
#;
argument {
P --> Q;
SOS |- (P || R) --> (Q || R);
}
#;
argument {
~P --> P;
SOS |- P;
}
#;
argument {
~P;
SOS |- P --> Q;
}
#;
argument {
P && Q;
SOS |- P --> Q;
}
#;
# SUPPLEMENTARY PROBLEMS VI IN SCHAUMS, PAGE 71.;
#;
argument {
SOS |- P --> P;
}
#;
argument {
SOS |- P --> (Q --> (P && Q));
}
#;
argument {
SOS |- ~(P <--> ~P);
}
#;
argument {
SOS |- (P --> Q) --> (~Q --> ~P);
}
#;
argument {
SOS |- (P && Q) || (~P || ~Q);
}
#;
argument {
SOS |- Q --> (P || ~P);
}
#;
argument {
SOS |- (P && ~P) --> Q;
}
#;
argument {
SOS |- P || (P --> Q);
}
#;
argument {
SOS |- ~P || (Q --> P);
}
#;
argument {
SOS |- (P --> Q) || (Q --> P);
}
#;
# SUPPLEMENTARY PROBLEMS VII IN SCHAUMS, PAGE 71.;
#;
argument {
SOS |- (P && Q) <--> ~(~P || ~Q);
}
#;
argument {
SOS |- (P || Q) <--> ~(~P && ~Q);
}
#;
argument {
SOS |- (P && Q) <--> ~(P --> ~Q);
}
#;
argument {
SOS |- (P || Q) <--> ~P --> Q;
}
#;
argument {
SOS |- P <--> ((P && Q) || (P && ~Q));
}
#;
argument {
SOS |- ~(P --> Q) <--> (P && ~Q);
}
#;
argument {
SOS |- (P <--> Q) <--> ((P && Q) || (~P && ~Q));
}
#;
argument {
SOS |- ~(P <--> Q) <--> ((~P && Q) || (P && ~Q));
}
#;
argument {
SOS |- (P && ~P) <--> (Q && ~Q);
}
#;
argument {
SOS |- (P || ~P) <--> (Q || ~Q);
}
#;
quit;
