#include <random/Random.h>
#include <adt/Set_BitVector.h>

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	MustBeTrue(argc == 4);

	unsigned long key = atol(argv[1]);
	setKey(key);

	int setsz = atoi(argv[2]);
	Set_BitVector s(setsz);
	cerr << "empty set is  ... " << s << endl;

	int mmax = atoi(argv[3]);
	for (int im = 1; im <= mmax; im++)
	{
		unsigned long newm = random();
		cerr << "new member is ... " << newm << endl;
		s += newm;
		cerr << "current set is  ... " << s << endl;
	}
	cerr << "final set is  ... " << s << endl;

	Set_BitVector s2;
	cerr << "empty set2 is  ... " << s2 << endl;

	s2 = s;
	cerr << "copy set to set2 ... " << s2 << endl;

	s2 -= s;
	cerr << "delete set from set2 ... " << s2 << endl;

	s2 = s;
	cerr << "copy set to set2 ... " << s2 << endl;

	Set_BitVector_Iterator s2it(s2);
	for ( ; ! s2it.done(); s2it++)
	{
		cerr << "iterator() ... " << s2it() << endl;
	}

	for (s2it.reset() ; ! s2it.done(); s2it++)
	{
		cerr << "removing with iterator() ... " << s2it() << endl;
		s2 -= s2it();
		cerr << "current set is  ... " << s2 << endl;
	}

	s2 = s;
	cerr << "copy set to set2 ... " << s2 << endl;

	for (s2it.reset() ; ! s2it.done(); s2it++)
	{
		if (random()%2 == 0)
		{
			cerr << "removing with iterator() ... " << s2it() << endl;
			s2 -= s2it();
			cerr << "current set is  ... " << s2 << endl;
		}
	}
	cerr << "final set2 is  ... " << s2 << endl;

	cerr << "--------------------------" << endl;
	cerr << "set1 is  ... " << s << endl;
	cerr << "set2 is  ... " << s2 << endl;
	cerr << "--------------------------" << endl;
	cerr << "s1 - s2 = " << (s-s2) << endl;
	cerr << "--------------------------" << endl;
	cerr << "s1 | s2 = " << (s|s2) << endl;
	cerr << "--------------------------" << endl;
	cerr << "s1 & s2 = " << (s&s2) << endl;
	cerr << "--------------------------" << endl;

	return(0);
}
