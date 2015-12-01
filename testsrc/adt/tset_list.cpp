//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "random/Random.h"
#include "adt/Set_List.h"

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	MustBeTrue(argc == 3);

	unsigned long key = atol(argv[1]);
	setKey(key);

	Set_List<int> s;
	cerr << "empty set is  ... " << s << endl;

	int mmax = atoi(argv[2]);
	for (int im = 1; im <= mmax; im++)
	{
		unsigned long newm = random();
		cerr << "new member is ... " << newm << endl;
		s += newm;
		cerr << "current set is  ... " << s << endl;
	}
	cerr << "final set is  ... " << s << endl;

	Set_List<int> s2;
	cerr << "empty set2 is  ... " << s2 << endl;

	s2 = s;
	cerr << "copy set to set2 ... " << s2 << endl;

	s2 -= s;
	cerr << "delete set from set2 ... " << s2 << endl;

	s2 = s;
	cerr << "copy set to set2 ... " << s2 << endl;

	Set_List_Iterator<int> s2it(s2);
	for ( ; ! s2it.done(); s2it++)
	{
		cerr << "iterator() ... " << s2it() << endl;
	}

	for (s2it.reset() ; ! s2it.done(); s2it++)
	{
		int tmp = s2it();
		cerr << "removing with iterator() ... " << s2it() << endl;
		s2 -= tmp;
		cerr << "current set is  ... " << s2 << endl;
	}

	s2 = s;
	cerr << "copy set to set2 ... " << s2 << endl;

	for (s2it.reset() ; ! s2it.done(); s2it++)
	{
		if (random()%2 == 0)
		{
			int tmp = s2it();
			cerr << "removing with iterator() ... " << s2it() << endl;
			s2 -= tmp;
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
