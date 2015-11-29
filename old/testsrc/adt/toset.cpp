// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <adt/Tuple.h>
#include <adt/OrderedSet.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	// check input
	MustBeTrue(argc > 1);

	// declare ordered set
	cout << "creating oset ..." << endl;
	OrderedSet<Tuple<int, int> > oset;

	// insert data
	cout << "inserting into oset ..." << endl;
	int data = 1;
	for (int arg = 1; arg < argc; arg++)
	{
		Tuple<int, int> tuple(atoi(argv[arg]), 0);
		for (int i = 1; i <= 5; i++, data++)
		{
			tuple.data = data;
			cout << "inserting tuple ... " << tuple << endl;
			if (oset.insert(tuple) != OK)
			{
				cout << "insert failed for tuple." << endl;					return(2);
			}
		}
	}

	// print data
	cout << "ordered set is ... " << oset << endl;

	// copy ordered set
	OrderedSet<Tuple<int, int> > oset2(oset);
	cout << "copied ordered set is ... " << oset << endl;

	// print using an iterator
	cout << "printing oset using an interator ..." << endl;
	OrderedSetIterator<Tuple<int, int> > osetIter(oset);
	for ( ; !osetIter.done(); osetIter++)
	{
		cout << "key is ... " << osetIter.key() << endl;
		cout << "data is ... " << osetIter.data() << endl;
	}

	// all done
	return(0);
}



