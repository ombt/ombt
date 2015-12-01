#;
# TEST CASES FROM CHAPTER 4 IN THE BOOK DEDUCTION: INTRODUCTORY ;
# SYMBOLIC LOGIC, BY D. BONEVAC.;
#;
#############################################################################;
#;
argument {
P || Q;
~P;
SOS |- Q;
}
#;
argument {
P || Q;
P --> R;
Q --> S;
SOS |- R || S;
}
#;
argument {
(P && Q) || (~P && ~Q);
SOS |- P <--> Q;
}
#;
argument {
~P || ~Q;
SOS |- Q --> ~P;
}
#;
argument {
~P || ~R;
SOS |- ~(P && R);
}
#;
argument {
P || Q;
~P || ~Q;
SOS |- P <--> ~Q;
}
#;
argument {
(P || Q) || R;
SOS |- P || (Q || R);
}
#;
argument {
P && (Q || R);
SOS |- (P && Q) || (P && R);
}
#;
argument {
(R && ~P) || (Q && R);
SOS |- (P --> Q) && R;
}
#;
argument {
P || Q;
R || S;
~(P || S);
SOS |- (Q && R) || P1;
}
#;
argument {
P;
~S || ~P;
P --> R;
SOS |- ~S && R;
}
#;
argument {
P || Q;
P --> R;
~R;
SOS |- Q;
}
#;
argument {
~P || Q;
~Q || R;
~R;
SOS |- ~P;
}
#;
argument {
(P && ~Q) --> ~R;
R;
SOS |- ~P || (P && Q);
}
#;
argument {
~S || (S && P);
(S --> P) --> R;
SOS |- R;
}
#;
argument {
P --> ~Q;
~P || R;
Q;
(Q && R) --> P;
SOS |- ~R;
}
#;
argument {
~(P && ~Q) || ~P;
SOS |- P --> Q;
}
#;
argument {
P && S;
P --> (~S || R);
SOS |- R;
}
#;
argument {
~P || Q;
~Q;
~P --> R;
SOS |- R;
}
#;
argument {
P --> Q;
R --> P;
SOS |- ~R || Q;
}
#;
argument {
R --> P;
~R --> Q;
Q --> S;
SOS |- P || S;
}
#;
argument {
P --> ~Q;
R;
R --> (Q || ~S);
SOS |- S --> ~P;
}
#;
argument {
P --> (~R || S);
P --> ~S;
SOS |- P --> ~R;
}
#;
argument {
P --> ~Q;
~P --> ~R;
R || ~S;
SOS |- ~Q || ~S;
}
#;
argument {
P && ~S;
R --> S;
P --> (Q || R);
SOS |- Q;
}
#;
argument {
P && Q;
R && ~S;
Q --> (P --> P1);
P1 --> (R --> (S || Q1));
SOS |- Q1;
}
#;
argument {
P && Q;
P --> (S || R);
~(R && Q);
SOS |- S;
}
#;
argument {
P || (Q || S);
S1 && ~S2;
~(~S1 || S2) --> ~P;
(S --> R) && ~R;
SOS |- Q;
}
#;
argument {
P && (~Q && ~P1);
P --> (S --> R);
S --> (R <--> (P1 || Q));
SOS |- ~S;
}
#;
argument {
S --> P;
(S && P) --> Q;
R --> S1;
R || S;
SOS |- Q || S1;
}
#;
argument {
P || (R || Q);
(R --> S1) && ( Q --> S2);
(S1 || S2) --> (P || Q);
~P;
SOS |- Q;
}
#;
argument {
P --> (S && R);
(R || ~S) --> (Q && Q1);
Q1 <--> Q2;
SOS |- P --> Q2;
}
#;
argument {
(P && Q) || (Q && R);
SOS |- ~Q --> S;
}
#;
argument {
(P || Q) && R;
Q --> S;
SOS |- ~P --> (R --> S);
}
#;
argument {
~P || (Q && R);
(R || ~Q) --> (S && Q1);
(Q1 && Q2) || ~(Q1 || Q2);
SOS |- ~P || Q2;
}
#;
argument {
~(~P && Q) && (P <--> ~Q);
SOS |- P <--> (Q --> R);
}
#;
quit;
