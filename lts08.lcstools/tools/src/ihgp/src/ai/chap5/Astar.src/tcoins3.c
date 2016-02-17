// test for best-first search code
#include <stdlib.h>
#include <iostream.h>
#include "coins.h"
#include "search.h"

// main entry point
int
main(int argc, char **argv)
{
	// check number of arguments
	if (argc != 2)
	{
		cerr << "usage: tcoins number" << endl;
		return(2);
	}

	// get number
	int number = atoi(argv[1]);

	// start state
	CoinsNode start(number);

	// best-first search
	int unique, expanded;
	if (BestFirst_Astar_Check(start, unique, expanded) == OK)
	{
		cout << "SOLUTION FOUND ..." << endl;
	}
	else
	{
		cout << "NO SOLUTION FOUND ..." << endl;
	}
	cout << "Unique nodes ... " << unique << endl;
	cout << "Expanded nodes ... " << expanded << endl;

	// all done
	return(0);
}
