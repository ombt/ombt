# PIGEON HOLE CLAUSES;
#;
argument {
P11 || P12;
P21 || P22;
P31 || P32;
~P11 || ~P21;
~P12 || ~P22;
~P11 || ~P31;
~P12 || ~P32;
~P21 || ~P31;
SOS |- ~P32 || ~P32;
}
#;
quit;
