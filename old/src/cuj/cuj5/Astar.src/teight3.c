// test for best-first search code
#include <stdlib.h>
#include <iostream.h>
#include "eight.h"
#include "search.h"

// main entry point
int
main(int argc, char **argv)
{
	// check number of arguments
	if (argc != 10 && argc != 19)
	{
		cerr << "usage: teight start state [goal state]" << endl;
		return(2);
	}

	// start state
	char b[3][3];
	b[0][0] = atoi(argv[1]);
	b[0][1] = atoi(argv[2]);
	b[0][2] = atoi(argv[3]);
	b[1][0] = atoi(argv[4]);
	b[1][1] = atoi(argv[5]);
	b[1][2] = atoi(argv[6]);
	b[2][0] = atoi(argv[7]);
	b[2][1] = atoi(argv[8]);
	b[2][2] = atoi(argv[9]);
	EightNode start(b);
	cout << "start state is ... " << start << endl;

	// goal state
	EightNode goal;
	if (argc == 19)
	{
		goal(0,0) = atoi(argv[10]);
		goal(0,1) = atoi(argv[11]);
		goal(0,2) = atoi(argv[12]);
		goal(1,0) = atoi(argv[13]);
		goal(1,1) = atoi(argv[14]);
		goal(1,2) = atoi(argv[15]);
		goal(2,0) = atoi(argv[16]);
		goal(2,1) = atoi(argv[17]);
		goal(2,2) = atoi(argv[18]);
	}
	cout << "goal state is ... " << goal << endl;

	// list of goal states
	List<EightNode> goals;
	goals.insertAtFront(goal);

	// best-first search
	int unique, expanded;
	if (BestFirst_Astar_Check(start, goals, unique, expanded) == OK)
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
