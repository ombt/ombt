#include "term.h"
#include "substitution.h"

main(int argc, char **argv)
{
	Substitutions stlist;
	for (int arg = 1; arg < argc && argv[arg][0] != '/'; arg += 2)
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
	cout << "substitutions [1] are ... " << stlist << endl;

	Substitutions stlist2;
	for (arg++ ; arg < argc; arg += 2)
	{
		Term t(argv[arg]);
		String v(argv[arg+1]);
		cout << "{ " << t << " / " << v << " } " << endl;
		if (stlist2.insert(Substitution(t, v)) != OK)
		{
			cout << "insert failed for variable ... " << v << endl;
			cout << "insert failed for term ... " << t << endl;
			return(2);
		}
	}
	cout << "substitutions [2] are ... " << stlist2 << endl;

	cout << "composition is ... " << stlist*stlist2 << endl;

	return(0);
}
