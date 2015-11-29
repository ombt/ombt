// test of unification algorithm
#include "literal.h"
#include "substitution.h"
#include "unification.h"

main(int argc, char **argv)
{
	Literal l1(argv[1]);
	Literal l1save(l1);
DUMP(l1);
DUMP((String)l1);

	Literal l2(argv[2]);
	Literal l2save(l2);
DUMP(l2);
DUMP((String)l2);

	Substitutions slist;

	switch (unify(l1, l2, slist))
	{
	case OK:
		cout << "====================================" << endl;
		cout << "L1 and L2 unify ... " << endl;
		cout << "L1 ... " << (String)l1 << endl;
		cout << "L2 ... " << (String)l2 << endl;
		cout << "substitutions ... " << slist << endl;
		slist.applyTo(l1save);
		cout << "L1 after substitutions ... " << (String)l1save << endl;
		slist.applyTo(l2save);
		cout << "L2 after substitutions ... " << (String)l2save << endl;
		break;

	case NOMATCH:
		cout << "====================================" << endl;
		cout << "L1 and L2 do NOT unify ... " << endl;
		cout << "L1 ... " << (String)l1 << endl;
		cout << "L2 ... " << (String)l2 << endl;
		break;

	default:
		cout << "an error ..." << endl;
		break;
	}

	return(0);
}
