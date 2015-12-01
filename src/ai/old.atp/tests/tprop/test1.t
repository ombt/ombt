argument {
~((A||B) && (C&&D));
~(A --> B);
~(A <--> B);
(~A <--> ~B);
~(~A <--> ~B);
SOS |- true;
}
quit;
