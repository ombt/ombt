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
~P || (Q && R);
(R || ~Q) --> (S && Q1);
(Q1 && Q2) || ~(Q1 || Q2);
SOS |- ~P || Q2;
}
#;
quit;
