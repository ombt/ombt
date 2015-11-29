// test of unification algorithm
#include "mylist.h"
#include "literal.h"
#include "substitution.h"
#include "unification.h"

main(int argc, char **argv)
{
	List<Literal> ll;

	for (int arg = 1; arg < argc; arg++)
	{
DUMP(argv[arg]);
		Literal larg(argv[arg]);
DUMP(larg);
		if (ll.insertAtEnd(larg) != OK)
		{
			cout << "insert failed for literal: " << larg << endl;
		}
DUMP(ll);
	}

	Substitutions slist;

	cout << "====================================" << endl;

	switch (unify(ll, slist))
	{
	case OK:
		cout << "literal did unify ... " << endl;
		cout << "literal list is ... " << ll << endl;
		cout << "substitutions are ... " << slist << endl;
		break;

	case NOMATCH:
		cout << "literal did NOT unify ... " << endl;
		cout << "literal list is ... " << ll << endl;
		cout << "substitutions are ... " << slist << endl;
		break;

	default:
		cout << "an error ..." << endl;
		break;
	}

	cout << "====================================" << endl;
	cout << "APPLY substitutions to literals in list ..." << endl;

	for (int arg = 1; arg < argc; arg++)
	{
		Literal larg(argv[arg]);
		cout << "literal BEFORE substitutions ... " << larg << endl;
		if (slist.applyTo(larg) != OK)
		{
			cout << "applying substitutions failed for literal: " << larg << endl;
		}
		cout << "literal AFTER substitutions ... " << larg << endl;
	}

	return(0);
}
