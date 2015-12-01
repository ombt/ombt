#
argument {
	LessThan(john, 5) --> Takes(john, a);
	~LessThan(john, 5) --> Takes(john, b);
	SOS |- [x]Takes(john, x);
}
#
argument {
	LessThan(john, 5) --> Takes(john, a);
	~LessThan(john, 5) --> Takes(john, b);
	SOS ?- [x]Takes(john, x);
}
#
quit;
#
