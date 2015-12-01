#
argument {
	forall(x)(L(x) --> D(x));
	forall(x)((D(x) && I(x)) --> S(x));
	L(c) && I(c);
	SOS |- S(c);
}
#
argument {
# this one should fail since there is a second existential
# quantifier in the forsome() case. the variable x is reused, 
# but it is not a free variable to bind.
	SOS |- forall(x)(A(x) --> B(x)) --> forsome(x)(A(x) --> forsome(x)B(x));
}
#
quit;
