#include "set_List.h"
#include "mystring.h"

main(int argc, char **argv)
{
	MustBeTrue(argc >= 5);

	int setAno = atoi(argv[1]);
	int setBno = atoi(argv[2]);

	String string;
	Set_List<String> setA;
	Set_List<String> setB;

	int arg = 3;
	for (int ia = 1; ia <= setAno && arg < argc; ia++, arg++)
	{
		setA.insert(String(argv[arg]));
	}
	cout << "setA is ... " << setA << endl;
	for (int ib = 1; ib <= setBno && arg < argc; ib++, arg++)
	{
		setB.insert(String(argv[arg]));
	}
	cout << "setB is ... " << setB << endl;

	if (setA.isSubsetOf(setB))
		cout << "setA is a subset of setB." << endl;
	else
		cout << "setA is NOT a subset of setB." << endl;
	if (setA.isSupersetOf(setB))
		cout << "setA is a superset of setB." << endl;
	else
		cout << "setA is NOT a superset of setB." << endl;
	if (setA.isProperSubsetOf(setB))
		cout << "setA is a proper subset of setB." << endl;
	else
		cout << "setA is NOT a proper subset of setB." << endl;
	if (setA.isProperSupersetOf(setB))
		cout << "setA is a proper superset of setB." << endl;
	else
		cout << "setA is NOT a proper superset of setB." << endl;

	return(0);
}
