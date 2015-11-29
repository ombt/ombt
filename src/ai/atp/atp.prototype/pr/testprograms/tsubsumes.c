// test of deletion functions
#include "literal.h"
#include "substitution.h"
#include "unification.h"
#include "clause.h"
#include "deletion.h"

main(int argc, char **argv)
{
	Clause c1, c2;
	int icl = 1;
	for (int arg = 1; arg < argc; arg++)
	{
		if (String(argv[arg]) == String("-"))
		{
			icl = 2;
			continue;
		}

		Literal l(argv[arg]);
DUMP(l);

		switch (icl)
		{
		case 1:
			if (c1.insert(l) != OK)
			{
				cout << "unable to insert literal " << l << endl;
				return(2);
			}
			break;
		case 2:
			if (c2.insert(l) != OK)
			{
				cout << "unable to insert literal " << l << endl;
				return(2);
			}
			break;
		default:
			MustBeTrue(icl == 1 || icl == 2);
		}
	}
DUMP(c1);
DUMP(c2);

	cout << "rename all variables in clauses ..." << endl;;
	MustBeTrue(c1.renameVariables() == OK);
	MustBeTrue(c2.renameVariables() == OK);
DUMP(c1);
DUMP(c2);

	Substitutions s;
	switch (subsumes(c1, c2, s))
	{
	case OK:
		cout << "====================================" << endl;
		cout << "c1 SUBSUMES c2 !!!" << endl;
		cout << "subs is ... " << s << endl;
		s.applyTo(c1);
		s.applyTo(c2);
		cout << "subs*c1 is ... " << c1 << endl;
		cout << "subs*c2 is ... " << c2 << endl;
		break;
	
	case NOMATCH:
		cout << "====================================" << endl;
		cout << "c1 DOES NOT SUBSUME c2 !!!" << endl;
		break;
	
	default:
		cout << "====================================" << endl;
		cout << "ERROR !!!" << endl;
		break;
	}

	return(0);
}
