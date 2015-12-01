//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "adt/Tuple.h"
#include "adt/OrderedSet.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	// check input
	MustBeTrue(argc > 1);

	// declare ordered set
TRACE();
	cout << "creating oset ..." << endl;
	OrderedSet<Tuple<int, int> > oset;

	// insert data
TRACE();
	cout << "INSERTING INTO OSET ..." << endl;
	int data = 1;
	for (int arg = 1; arg < argc; arg++)
	{
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		for (int i = 1; i <= 5; i++, data++)
		{
			tuple.data = data;
TRACE();
			cout << "inserting tuple ... " << tuple << endl;
			if (oset.insert(tuple) != OK)
			{
TRACE();
				cout << "insert failed for tuple." << endl;
				return(2);
			}
		}
	}

	// print data
TRACE();
	cout << "ordered set is ... " << oset << endl;

	// report counts
TRACE();
	cout << "REPORTING COUNTS IN OSET ..." << endl;
	for (int arg = 1; arg < argc; arg++)
	{
		int argvalue = atoi(argv[arg]);
		Tuple<int, int> tuple(argvalue, 0);
TRACE();
		cout << "oset counts for " << argvalue << endl;
TRACE();
		cout << "counts are ... " << oset.getSize(tuple) << endl;
	}
TRACE();
	cout << "total counts are ... " << oset.getSize() << endl;

	// updating data
TRACE();
	cout << "UPDATING OSET ..." << endl;
	for (int arg = 1; arg < argc; arg++)
	{
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		for (int i = 1; i <= 5; i++, data++)
		{
			tuple.data = data;
TRACE();
			cout << "updating tuple ... " << tuple << endl;
			if (oset.update(tuple, i) != OK)
			{
TRACE();
				cout << "update failed for tuple." << endl;
				return(2);
			}
		}
	}

	// print data
TRACE();
	cout << "ordered set is ... " << oset << endl;

	// declare ordered set
TRACE();
	cout << "creating copied oset ..." << endl;
	OrderedSet<Tuple<int, int> > oset2(oset);

	// print data
TRACE();
	cout << "copied ordered set is ... " << oset2 << endl;

	// updating data
TRACE();
	cout << "UPDATING COPIED OSET ..." << endl;
	for (int arg = 1; arg < argc; arg++)
	{
TRACE();
		cout << "retrieving data from copied set." << endl;
		List<Tuple<int, int> > tlist;
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		if (oset2.retrieve(tuple, tlist) != OK)
		{
TRACE();
			cout << "retrieve failed for copied oset." << endl;
			return(2);
		}
		for (int i=1; !tlist.isEmpty(); i++)
		{
			if (tlist.removeAtEnd(tuple) != OK)
			{
TRACE();
				cout << "remove at end failed." << endl;
				return(2);
			}
TRACE();
			cout << "updating tuple ... " << tuple << endl;
			if (oset2.update(tuple, i) != OK)
			{
TRACE();
				cout << "update failed for tuple." << endl;
				return(2);
			}
TRACE();
			cout << "counts are ... " << oset2.getSize(tuple) << endl;
		}
	}
TRACE();
	cout << "UPDATED COPIED ORDERED SET IS ... " << oset2 << endl;
TRACE();
	cout << "total counts are ... " << oset2.getSize() << endl;

	// updating data
TRACE();
	cout << "UPDATING COPIED OSET (2) ..." << endl;
	for (int arg = 1; arg < argc; arg++)
	{
TRACE();
		cout << "retrieving data from copied set." << endl;
		List<Tuple<int, int> > tlist;
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		if (oset2.retrieve(tuple, tlist) != OK)
		{
TRACE();
			cout << "retrieve failed for copied oset." << endl;
			return(2);
		}
		tlist.clear();
TRACE();
		cout << "updating clause ... " << tuple << endl;
		if (oset2.update(tuple, tlist) != OK)
		{
TRACE();
			cout << "update failed for tuple." << endl;
			return(2);
		}
TRACE();
		cout << "counts are ... " << oset2.getSize(tuple) << endl;
	}
TRACE();
	cout << "UPDATED COPIED ORDERED SET (2) IS ... " << oset2 << endl;
TRACE();
	cout << "total counts are ... " << oset2.getSize() << endl;

	// assignment
	oset2 = oset;
TRACE();
	cout << "ASSIGNED COPIED ORDERED SET (3) IS ... " << oset2 << endl;

	// removing data
TRACE();
	cout << "REMOVING COPIED OSET (2) ..." << endl;
	for (int arg = 1; arg < argc; arg++)
	{
TRACE();
		cout << "retrieving data from copied set." << endl;
		List<Tuple<int, int> > tlist;
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		if (oset2.retrieve(tuple, tlist) != OK)
		{
TRACE();
			cout << "retrieve failed for copied oset." << endl;
			return(2);
		}
TRACE();
		cout << "removing clause ... " << tuple << endl;
		if (oset2.remove(tuple, 2) != OK)
		{
TRACE();
			cout << "remove (2) failed for tuple." << endl;
			return(2);
		}
		if (oset2.remove(tuple, 4) != OK)
		{
TRACE();
			cout << "remove (4) failed for tuple." << endl;
			return(2);
		}
TRACE();
		cout << "counts are ... " << oset2.getSize(tuple) << endl;
	}
TRACE();
	cout << "REMOVED COPIED ORDERED SET (1) IS ... " << oset2 << endl;
TRACE();
	cout << "total counts are ... " << oset2.getSize() << endl;

	// removing data
TRACE();
	cout << "REMOVING ALL COPIED OSET (2) ..." << endl;
	for (int arg = 1; arg < (argc-1); arg++)
	{
TRACE();
		cout << "removing clause ... " << endl;
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		if (oset2.remove(tuple) != OK)
		{
TRACE();
			cout << "remove failed for tuple." << endl;
			return(2);
		}
TRACE();
		cout << "counts are ... " << oset2.getSize(tuple) << endl;
	}
TRACE();
	cout << "REMOVED COPIED ORDERED SET (2) IS ... " << oset2 << endl;
TRACE();
	cout << "total counts are ... " << oset2.getSize() << endl;

	// all done
	return(0);
}



