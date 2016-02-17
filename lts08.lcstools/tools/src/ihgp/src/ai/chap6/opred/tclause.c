#include "term.h"
#include "clause.h"
#include "list.h"

main(int argc, char **argv)
{
	Clause clause1;
	int arg2 = argc/2+1;
	for (int arg = 1; arg < arg2; arg++)
	{
		cout << "inserting ... " << argv[arg] << endl;
		clause1.insert(Term(argv[arg]));
	}
	cout << "clause 1 is ... " << clause1 << endl;

	Clause clause2;
	for (arg = arg2; arg < argc; arg++)
	{
		cout << "inserting ... " << argv[arg] << endl;
		clause2.insert(Term(argv[arg]));
	}
	cout << "clause 2 is ... " << clause2 << endl;

	cout << "clause1 == clause2 ? ... ";
	if (clause1 == clause2)
		cout << "TRUE" << endl;
	else
		cout << "FALSE" << endl;

	return(0);
}
