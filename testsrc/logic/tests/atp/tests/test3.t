argument {
~P || (Q && R);
(R || ~Q) --> (S && Q1);
(Q1 && Q2) || ~(Q1 || Q2);
|- ~P || Q2;
}
quit;