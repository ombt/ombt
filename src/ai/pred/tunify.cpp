#include "term.h"
#include "substitution.h"
#include "unification.h"

main(int argc, char **argv)
{
	Term t1(argv[1]);
	Term t1save(t1);
DUMP(t1);

	Term t2(argv[2]);
	Term t2save(t2);
DUMP(t2);

	Substitutions stlist;

	switch (unify(t1, t2, stlist))
	{
	case OK:
		cout << "t1 and t2 unify ... " << endl;
		cout << "t1 ... " << t1 << endl;
		cout << "t2 ... " << t2 << endl;
		cout << "stlist ... " << stlist << endl;
		stlist.applyTo(t1save);
		cout << "t1 after substitutions ... " << t1save << endl;
		stlist.applyTo(t2save);
		cout << "t2 after substitutions ... " << t2save << endl;
		break;

	case NOMATCH:
		cout << "t1 and t2 do NOT unify ... " << endl;
		cout << "t1 ... " << t1 << endl;
		cout << "t2 ... " << t2 << endl;
		break;

	default:
		cout << "an error ..." << endl;
		break;
	}

	return(0);
}
