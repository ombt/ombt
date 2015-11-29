// test for best-first search code
#include <stdlib.h>
#include <iostream.h>
#include "queen.h"
#include "search.h"

// main entry point
int
main(int argc, char **argv)
{
	// check number of arguments
	if (argc != 4)
	{
		cerr << "usage: tqueen rows cols nqueens" << endl;
		return(2);
	}

	// get number of columns and rows
	int nrows = atoi(argv[1]);
	int ncols = atoi(argv[2]);

	// get number of queens
	int nqueens = atoi(argv[3]);

	// start state
	NQueenNode start(nqueens, nrows, ncols);

	// best-first search
	int unique, expanded;
	if (BestFirst_Astar_WOCheck(start, unique, expanded) == OK)
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
