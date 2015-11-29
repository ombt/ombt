#
#
#
# default values for heuristic options.
#
options {
	# gvalue constants - distance from a start node
	gconst1 = 1;
	gconst2 = 0;
	gconst3 = 0;

	# hvalue constants - unit-preference estimate
	uconst1 = 1;
	uconst2 = 0;
	uconst3 = 0;

	# hvalue constants - complexity estimate
	cconst1 = 0;
	cconst2 = 0;
	cconst3 = 0;
	cconst4 = 0;
	cconst5 = 0;
	cconst6 = 0;
	cconst7 = 0;
}
#
#
argument {
A;
A --> B;
B --> C;
SOS |- C;
}
#
