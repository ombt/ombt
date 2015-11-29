# TEST FOR DEMORGANS;
argument {
~(A && B);
~~(A && B);
~(A || B);
~~(A || B);
~(~A && ~~B);
~~(~A && ~~B);
~(~~A || ~B);
~~(~~A || ~B);
SOS |- true;
}
quit;
