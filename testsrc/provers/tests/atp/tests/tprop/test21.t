#;
# PROBLEMS FROM DEDUCTION: INTRODUCTORY SYMBOLIC LOGIC BY;
# DANIEL BONEVAC, CHAPTER 4.;
#;
argument {
(P && ~R) <--> (S || ~Q);
S1 && ((~S && ~R) --> P);
((S1 --> Q) || (S1 --> R)) && (((P && Q) && S) --> R);
SOS |- Q && R;
}
#;
quit;
