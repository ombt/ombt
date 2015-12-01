argument {
(C --> P) --> D;
C --> (~M && D);
(B || D) --> P;
SOS |- D;
}
#;
argument {
(C --> P) --> D;
C --> (~M && D);
(B || D) --> P;
SOS |- (B || D) && C;
}
#;
argument {
(C --> P) --> D;
C --> (~M && D);
(B || D) --> P;
SOS |- P;
}
#;
quit;
