// test for best-first AO* search code
#include <stdlib.h>
#include <iostream.h>
#include "search.h"

// main entry point
int
main(int , char **)
{
	// best-first search
	Node start;
	if (BestFirst_AOstar(start) == OK)
	{
		cout << "SOLUTION FOUND ..." << endl;
	}
	else
	{
		cout << "NO SOLUTION FOUND ..." << endl;
	}

	// all done
	return(0);
}
