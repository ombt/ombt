#;
# PROBLEMS FROM DEDUCTION: INTRODUCTORY SYMBOLIC LOGIC BY;
# DANIEL BONEVAC, CHAPTER 4.;
#;
argument {
~(P --> ~Q);
R --> (~P || ~Q);
(R || S) <--> S1;
SOS |- S1 <--> S;
}
#;
argument {
~(~P && Q);
(P || Q) --> ~(Q1 && S);
Q <--> S;
SOS |- Q1 --> ~(Q || S);
}
#;
argument {
P --> (Q || R);
(~Q && Q1) || (S --> P);
~(~R --> ~P);
SOS |- ~S || Q;
}
#;
argument {
P --> (Q --> R);
(~Q <--> S) --> ~P;
P || Q1;
SOS |- (~R && S) --> Q1;
}
#;
argument {
(P <--> Q) <--> R;
~(P <--> ~R);
(Q || ~S) --> S1;
SOS |- S1;
}
#;
argument {
~S --> ~S2;
(S && S1) --> (P <--> Q);
~(~P || Q);
SOS |- S1 --> ~S2;
}
#;
argument {
(Q <--> ~P) --> ~R;
(~Q && S) || (P && Q1);
(S || Q1) --> R;
SOS |- P --> Q;
}
#;
argument {
(P && ~R) <--> (S || ~Q);
S1 && ((~S && ~R) --> P);
((S1 --> Q) || (S1 --> R)) && (((P && Q) && S) --> R);
SOS |- Q && R;
}
#;
quit;
