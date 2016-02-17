// test for priority queue 

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "pqueue.h"
#include "uniform.h"

main(int , char **argv)
{
	cout << "PriorityQueue pqueue;" << endl;
	PriorityQueue pqueue;
	cout << pqueue << endl;

	cout << "Uniform ud(1.0, 500.0);" << endl;
	Uniform ud(1.0, 500.0);
	ud.setSeed(atol(argv[1]));

	cout << "insert(pqueue, dataitem);" << endl;
	int ihmax = atoi(argv[2]);
	for (int ih = 1; ih <= ihmax; ih++)
	{
		DataItem *pd= new DataItem(long(ud.randomSample()));
		assert(pd != (DataItem *)0);
		insert(pqueue, pd);
	}
	cout << pqueue << endl;

	cout << "delete(pqueue, dataitem);" << endl;
	while ( ! isEmpty(pqueue))
	{
		DataItem *pd;
		remove(pqueue, pd);
		cout << *pd << endl;
	}
	
        // all done
        return(0);
}

