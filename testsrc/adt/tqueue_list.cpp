//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdlib.h>
#include <iostream>
#include "adt/String.h"
#include "adt/Queue_List.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	int arg;
	
	// size of queue
	Queue_List<String> queue;

	// insert data items
	for (arg = 2; arg < argc; arg++)
	{
		MustBeTrue(queue.enqueue(String(argv[arg])) == OK);
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
