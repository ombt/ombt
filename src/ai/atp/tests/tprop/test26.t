#;
# TEST CASES FROM CHAPTER 4 IN THE BOOK DEDUCTION: INTRODUCTORY ;
# SYMBOLIC LOGIC, BY D. BONEVAC.;
#;
#############################################################################;
#;
argument {
P && Q;
SOS |- Q;
}
#;
argument {
P && ( Q && R);
SOS |- Q;
}
#;
argument {
SOS |- ~(P && ~P);
}
#;
argument {
SOS |- ~(P && (Q && ~P));
}
#;
argument {
SOS |- ~((P && ~Q) && (Q && ~P));
}
#;
argument {
~P && ~Q;
SOS |- ~(P && Q);
}
#;
argument {
(P && Q) && R;
SOS |- P && (Q && R);
}
#;
argument {
P && ~Q;
~R && S;
SOS |- S && P;
}
#;
argument {
~( P && Q);
Q;
SOS |- ~P;
}
#;
argument {
Q;
SOS |- ~(~P && ~Q);
}
#;
argument {
P;
SOS |- ~(Q && ~(P && Q));
}
#;
argument {
~((P && ~Q) && R);
SOS |- ~((P && R) && ~Q);
}
#;
argument {
~(P && ~Q);
P;
SOS |- Q;
}
#;
argument {
~(~P && Q);
~(P && Q);
SOS |- ~Q;
}
#;
argument {
P && Q;
~(Q && ~R);
~(P && ~S);
SOS |- R && S;
}
#;
argument {
SOS |- ~(~(~(P && ~Q) && ~P) && ~P);
}
#;
argument {
P && Q;
~(~R && Q);
~(R && ~S);
SOS |- P && S;
}
#;
# IN THE FOLLOWING PROBLEM, THE CONCLUSION "R" NEVER APPEARS ANYWHERE;
# IN THE ASSUMPTIONS. IS THIS A TYPO???;
#;
argument {
~(P && ~Q);
~(Q && ~P);
~(P && Q);
~(~P && ~Q);
SOS |- R;
}
#;
argument {
~(~S && Q);
~(P && (~Q && ~R));
~(R && ~S);
SOS |- ~(~S && P);
}
#;
# THIS ONE FAILS SINCE THE CONCLUSION DOES NOT APPEAR ANYWHERE IN THE;
# ASSUMPTIONS. NEED TO CHECK THIS ONE OUT.;
#;
argument {
~(P && R);
~(~(P && Q) && ~P);
R;
SOS |- ~S;
}
#;
quit;
