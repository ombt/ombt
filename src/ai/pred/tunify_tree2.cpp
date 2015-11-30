#include "term_tree.h"

main(int argc, char **argv)
{
	Term t1(argv[1]);
DUMP(t1);
	Term t2(argv[2]);
DUMP(t2);
	Term t3(argv[3]);
DUMP(t3);

	cout << "ucompare(t1, t2) ... ";
	int status = ucompare(t1, t2);
	if (status == 0)
		cout << "t1 == t2" << endl;
	else if (status < 0)
		cout << "t1 < t2" << endl;
	else
		cout << "t1 > t2" << endl;

	cout << "ucompare(t2, t3) ... ";
	status = ucompare(t2, t3);
	if (status == 0)
		cout << "t2 == t3" << endl;
	else if (status < 0)
		cout << "t2 < t3" << endl;
	else
		cout << "t2 > t3" << endl;

	cout << "ucompare(t1, t3) ... ";
	status = ucompare(t1, t3);
	if (status == 0)
		cout << "t1 == t3" << endl;
	else if (status < 0)
		cout << "t1 < t3" << endl;
	else
		cout << "t1 > t3" << endl;

	return(0);
}
