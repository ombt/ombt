#include "term.h"
#include "substitution.h"

main(int argc, char **argv)
{
	Term term(argv[1]);
	cout << "Term is ... " << term << endl;

	Substitutions stlist;
	for (int arg = 2; arg < argc; arg += 2)
	{
		Term t(argv[arg]);
		String v(argv[arg+1]);
		cout << "{ " << t << " / " << v << " } " << endl;
		if (stlist.insert(Substitution(t, v)) != OK)
		{
			cout << "insert failed for variable ... " << v << endl;
			cout << "insert failed for term ... " << t << endl;
			return(2);
		}
	}
	cout << "substitutions are ... " << stlist << endl;

	if (stlist.applyTo(term) != OK)
	{
		cout << "applying substitutions to term failed ... " << term << endl;
		return(2);
	}
	cout << "term after substitutions is ... " << term << endl;
	return(0);
}
