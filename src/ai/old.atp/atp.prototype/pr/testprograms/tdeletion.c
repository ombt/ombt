// test of deletion functions
#include "literal.h"
#include "substitution.h"
#include "unification.h"
#include "clause.h"
#include "deletion.h"

main(int argc, char **argv)
{
	Clause c;
	for (int arg = 1; arg < argc; arg++)
	{
		Literal l(argv[arg]);
DUMP(l);
		if (c.insert(Literal(argv[arg])) != OK)
		{
			cout << "unable to insert literal " << l << endl;
			return(2);
		}
	}
DUMP(c);

	switch (tautology(c))
	{
	case OK:
		cout << "====================================" << endl;
		cout << "clause is a tautology !!!" << endl;
		break;

	case NOMATCH:
		cout << "====================================" << endl;
		cout << "clause is NOT a tautology !!!" << endl;
		break;

	default:
		cout << "====================================" << endl;
		cout << "ERROR !!!" << endl;
		break;
	}

	return(0);
}
