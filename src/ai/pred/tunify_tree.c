#include "term_tree.h"

main(int argc, char **argv)
{
	Term t1(argv[1]);
	Term t1save(t1);
DUMP(t1);

	Term t2(argv[2]);
	Term t2save(t2);
DUMP(t2);

	cout << "ucompare(t1, t2) ... ";
	int status = ucompare(t1, t2);
	if (status == 0)
		cout << "t1 == t2" << endl;
	else if (status < 0)
		cout << "t1 < t2" << endl;
	else
		cout << "t1 > t2" << endl;

	cout << "ucompare(t2, t1) ... ";
	status = ucompare(t2, t1);
	if (status == 0)
		cout << "t2 == t1" << endl;
	else if (status < 0)
		cout << "t2 < t1" << endl;
	else
		cout << "t2 > t1" << endl;

	cout << "compare(t1, t2) ... ";
	status = compare(t1, t2);
	if (status == 0)
		cout << "t1 == t2" << endl;
	else if (status < 0)
		cout << "t1 < t2" << endl;
	else
		cout << "t1 > t2" << endl;

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

	status = compare(t1save, t2save);
	if (status == 0)
		cout << "t1 == t2" << endl;
	else if (status < 0)
		cout << "t1 < t2" << endl;
	else
		cout << "t1 > t2" << endl;

	return(0);
}
