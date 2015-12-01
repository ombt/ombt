
#
argument {
	Man(confucius);
	forall(x)(Man(x)-->Mortal(x));
	SOS ?- forsome(x)Mortal(x);
}
#
argument {
	Man(confucius);
	forall(x)(Man(x)-->Mortal(x));
	SOS ?- Mortal(confucius);
}
#
quit;
#

