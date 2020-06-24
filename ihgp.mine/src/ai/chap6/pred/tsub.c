#include "term.h"
#include "substitution.h"

main(int argc, char **argv)
{
	String s1(argv[1]);
	cout << "string 1 is ... " << s1 << endl;

	Term t1(argv[2]);
	cout << "term 1 is ... " << t1 << endl;

	Substitution st1(t1, s1);
	cout << "substitution 1 is ... " << st1 << endl;

	String s2(argv[3]);
	cout << "string 2 is ... " << s2 << endl;

	Term t2(argv[4]);
	cout << "term 2 is ... " << t2 << endl;

	Substitution st2(t2, s2);
	cout << "substitution 2 is ... " << st2 << endl;

	cout << "composition st1*st2 is ... " << st1*st2 << endl;

	Substitutions stlist;
	cout << "substitutions is ... " << stlist << endl;

	stlist.insert(st1);
	cout << "substitutions is ... " << stlist << endl;

	stlist.insert(st2);
	cout << "substitutions is ... " << stlist << endl;

	cout << "composition of substitutions is ... " << stlist*stlist << endl;

	return(0);
}
