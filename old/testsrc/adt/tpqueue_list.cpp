// headers
#include <stdlib.h>
#include <iostream>
#include <adt/String.h>
#include <adt/PriorityQueue_List.h>
#include <random/Random.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	int arg;
	
	// check options
	MustBeTrue(argc >= 3);

	// cmd args
	int nchars = atoi(argv[1]);
	unsigned long seed = atol(argv[2]);
	
	// size of queue
	PriorityQueue_List<String> queue;

	// seed for random number generator
	setKey(seed);

	// insert data items
	for (int ichar = 1; ichar <= nchars; ichar++)
	{
		char tmpchar = myrandomchar();
		String tmp(tmpchar);
		cout << "queued item is ... " << tmp << endl;
		MustBeTrue(queue.enqueue(tmp) == OK);
	}
	cout << "queue after inserts is ... " << queue << endl;
	while (!queue.isEmpty())
	{
		String tmp;
		MustBeTrue(queue.dequeue(tmp) == OK);
		cout << "dequeued item is ... " << tmp << endl;
	}
	cout << "queue after deletes is ... " << queue << endl;

	return(0);
}
