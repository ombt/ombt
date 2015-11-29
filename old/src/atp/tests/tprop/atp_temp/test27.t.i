#;
# TEST CASES FROM CHAPTER 4 IN THE BOOK DEDUCTION: INTRODUCTORY ;
# SYMBOLIC LOGIC, BY D. BONEVAC.;
#;
#############################################################################;
#;
argument {
P --> Q;
Q --> R;
SOS |- P --> R;
}
#;
argument {
P --> Q;
Q --> R;
SOS |- ~R --> ~P;
}
#;
argument {
P --> Q;
~P --> R;
SOS |- ~Q --> R;
}
#;
argument {
P --> Q;
~P --> R;
SOS |- ~R --> Q;
}
#;
argument {
P --> R;
Q --> ~R;
SOS |- P --> ~Q;
}
#;
argument {
P <--> (Q && P);
SOS |- P --> Q;
}
#;
argument {
P --> Q;
~(P --> R);
SOS |- ~(Q --> R);
}
#;
argument {
P --> Q;
P --> R;
SOS |- P --> (Q && R);
}
#;
argument {
P <--> Q;
P <--> R;
SOS |- Q <--> R;
}
#;
argument {
P --> Q;
~Q;
SOS |- ~P;
}
#;
argument {
P <--> Q;
~P;
SOS |- ~Q;
}
#;
argument {
(P --> Q) --> P;
SOS |- P;
}
#;
argument {
P --> Q;
SOS |- P <--> (P && Q);
}
#;
argument {
P && ~Q;
R --> (R && Q);
~R --> S;
SOS |- S;
}
#;
argument {
S --> (R && P);
Q --> (~R && ~P1);
SOS |- (Q && S) --> P2;
}
#;
argument {
P --> (Q && (~R --> ~P));
Q <--> ~R;
SOS |- ~P;
}
#;
argument {
(P --> Q) --> (R --> S);
~(P && Q) --> S;
SOS |- ~S --> ~R;
}
#;
argument {
(S && ~R) --> ~P;
(Q --> ~S) <--> ~P;
SOS |- P <--> (R && (S && Q));
}
#;
argument {
P <--> Q;
SOS |- ~P <--> ~Q;
}
#;
argument {
(P <--> Q) <--> (P <--> R);
SOS |- (Q <--> R);
}
#;
quit;
