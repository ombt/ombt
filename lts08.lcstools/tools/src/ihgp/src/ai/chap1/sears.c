//
// find the quickest route from one floor to another via elevators
// in a tall building. there are 3 types of elevators: express
// that go directly from one floor to another floor; local elevators
// that service a range of floors, and service elevators that
// service a range of floors, but are slower than locals.
// 
// each type of elevator takes a certain amount of time per floor.
//
// if an elevator can service two floors, set the entry in the table
// equal to the transit time.
//
//////////////////////////////////////////////////////////////////////////////
//
// headers
#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <assert.h>
#include <limits.h>

// local headers
#include "debug.h"

// definitions
#define Express 0
#define Local (Express+1)
#define Service (Local+1)
#define ElevatorTypes (Service+1)

// type definitions
typedef unsigned int uint;

// structures
struct Table {
	uint time;
};

// tables with data and globals
Table **express = NULL;
Table **local = NULL;
Table **service = NULL;
Table **tables[ElevatorTypes];
uint costs[ElevatorTypes]; 
uint floors = 0;
uint vflag = 0;

// functions for simple sets, store size in element 0.
void
CreateSet(uint *&set, uint setsz, uint initval = 0)
{
	// check set size
	assert(setsz > 0);

	// allocate set
	set = new uint [setsz+1];
	assert(set != NULL);

	// initialize to empty set
	for (int i = 0; i <= setsz; i++)
	{
		set[i] = initval;
	}
	set[0] = setsz;

	// all done
	return;
}

// insert a set member
void
InsertIntoSet(uint *set, uint member)
{
	assert(set != NULL);
	assert(1 <= member && member <= set[0]);
	set[member] = 1;
	return;
}

// delete a set member
void
DeleteFromSet(uint *set, uint member)
{
	assert(set != NULL);
	assert(1 <= member && member <= set[0]);
	set[member] = 0;
	return;
}

// check if in set
int
IsInSet(uint *set, uint member)
{
	assert(set != NULL);
	assert(1 <= member && member <= set[0]);
	return(set[member]);
}
	
// step through set
int
FindFirstMember(uint *set, uint &cursor)
{
	for (cursor=1; cursor <= set[0] && !set[cursor]; cursor++);
	if (cursor <= set[0])
		return(cursor);
	else 
		return(0);
}

int
FindNextMember(uint *set, uint &cursor)
{
	for (cursor++; cursor <= set[0] && !set[cursor]; cursor++);
	if (cursor <= set[0])
		return(cursor);
	else 
		return(0);
}

// dump set data
void
DumpSet(char *msg, uint *set)
{
	cerr << msg;
	for (int i = 1; i <= set[0]; i++)
	{
		cerr << set[i] << " ";
		if (i%20 == 0)
			cerr << endl;
	}
	if (i%20 != 0)
		cerr << endl;
	return;
}

// read in table and populate tables
int
GetFloorData(char *fname, int &floors)
{
	// input stream
	filebuf *fbp = NULL;
	istream_withassign fin;

	// open and read file
	if (fname != NULL)
	{
		fbp = new filebuf;
		assert(fbp != NULL);
		if (fbp->open(fname, ios::in) == 0)
			return(-1);
		fin = fbp;
	}
	else
		fin = cin.rdbuf();
	
	// read in data
	cout << "enter number of floors: " << endl;
	fin >> floors;
	if (floors < 1)
	{
		cerr << "less than one floor." << endl;
		if (fbp != NULL) delete fbp;
		return(-1);
	}

	// get costs per floor
	cout << "enter cost (time/floor) for express: " << endl;
	fin >> costs[Express];
	cout << "costs[Express] is ... " << costs[Express] << endl;
	cout << "enter cost (time/floor) for local: " << endl;
	fin >> costs[Local];
	cout << "costs[Local] is ... " << costs[Local] << endl;
	cout << "enter cost (time/floor) for service: " << endl;
	fin >> costs[Service];
	cout << "costs[Service] is ... " << costs[Service] << endl;

	// allocate matrices
	express = new Table *[floors+1];
	assert(express != NULL);
	local = new Table *[floors+1];
	assert(local != NULL);
	service = new Table *[floors+1];
	assert(service != NULL);
	for (int i = 1; i <= floors; i++)
	{
		express[i] = new Table[floors+1];
		assert(express[i] != NULL);
		local[i] = new Table[floors+1];
		assert(local[i] != NULL);
		service[i] = new Table[floors+1];
		assert(service[i] != NULL);
	}
	tables[Express] = express;
	tables[Local] = local;
	tables[Service] = service;

	// initialize
	for (i = 0; i < ElevatorTypes; i++)
	{
		for (int j = 1; j <= floors; j++)
		{
			for (int k = 1; k <= floors; k++)
			{
				tables[i][j][k].time = INT_MAX;
			}
			tables[i][j][j].time = 0;
		}
	}
	
	// read in floor data
	char etype[16];
	int f1, f2;
	for (int nrecords = 0; ! fin.eof(); nrecords++)
	{
		// read record
		cout << "enter elevator type (e,l,s), f1, f2" << endl;
		fin >> etype >> f1 >> f2;
		if (f1 == f2)
		{
			cout << "floor1 == floor2" << endl;
			cout << "skipping floor " << f1 << endl;
			continue;
		}
		else if (f2 < f1)
		{
			int tmp = f1;
			f1 = f2;
			f2 = tmp;
		}

		// switch statemente
		switch (etype[0])
		{
		case 'e':
		case 'E':
			tables[Express][f1][f2].time = 
				(f2 - f1)*costs[Express];
			tables[Express][f2][f1].time = 
				tables[Express][f1][f2].time;
			break;

		case 'l':
		case 'L':
		{
			for (int if1 = f1; if1 <= f2; if1++)
			{
				for (int if2 = if1+1; if2 <= f2; if2++)
				{
					tables[Local][if1][if2].time = 
						(if2 - if1)*costs[Local];
					tables[Local][if2][if1].time = 
						tables[Local][if1][if2].time;
				}
			}
			break;
		}

		case 's':
		case 'S':
		{
			for (int if1 = f1; if1 <= f2; if1++)
			{
				for (int if2 = if1+1; if2 <= f2; if2++)
				{
					tables[Service][if1][if2].time = 
						(if2 - if1)*costs[Service];
					tables[Service][if2][if1].time = 
						tables[Service][if1][if2].time;
				}
			}
			break;
		}
		}
	}

	// create one table with the shortest time for each
	// transition. use express table for shortest times.
	for (int j = 1; j <= floors; j++)
	{
		for (int k = 1; k <= floors; k++)
		{
			int min = Express;
			if (tables[Local][j][k].time < tables[min][j][k].time)
				min = Local;
			if (tables[Service][j][k].time < tables[min][j][k].time)
				min = Service;
			tables[Express][j][k].time = tables[min][j][k].time;
		}
	}

	// print out table
	if (vflag)
	{
		j = 1;
		cerr << setw(10) << j++;
		for ( ; j <= floors; j++)
			cerr << setw(5) << j;
		cerr << endl;
		for (j = 1; j <= floors; j++)
		{
			cerr << setw(3) << j << ": ";
			for (int k = 1; k <= floors; k++)
			{
				if (tables[Express][j][k].time == INT_MAX)
				{
					cerr << setw(5) << "BIG";
				}
				else
				{
					cerr << setw(5) << tables[Express][j][k].time;
				}
			}
			cerr << endl;
		}
		cerr << endl;
	}

	// all done
	if (fbp != NULL) delete fbp;
	return(0);
}

// figure out the least cost path from floor1 to floor2 using
// dijkstra's algorithm.
void
GetLeastTimePath(uint nf, uint f1, uint f2)
{
	// sets 
	uint *s, *d, *v, *path;
	CreateSet(s, nf, 0);
	CreateSet(v, nf, 1);
	CreateSet(d, nf, 0);
	CreateSet(path, nf, 0);

	// initialize costs matrix and sets
	for (int i = 1; i <= nf; i++)
	{
		d[i] = tables[Express][f1][i].time;
		if (d[i] != INT_MAX)
			path[i] = f1;
		else
			path[i] = 0;
	}
	InsertIntoSet(s, f1);
	DeleteFromSet(v, f1);
	path[f1] = 0;
	if (vflag)
	{
		DumpSet("1, S = ", s);
		DumpSet("1, V = ", v);
	}

	// generate paths
	for (i = 2; i <= nf; i++)
	{
		// verify output
		if (vflag)
		{
			cerr << "ITERATION = " << i << endl;
			DumpSet("2, S = ", s);
			DumpSet("2, V = ", v);
			DumpSet("2, D = ", d);
			DumpSet("2, PATH = ", path);
		}

		// find the node with the minimum distance in d[] and in V.
		uint w, x, vsave;
		for (w = FindFirstMember(v, vsave),
		     x = FindNextMember(v, vsave);
		     x;
		     x = FindNextMember(v, vsave))
		{
			if (d[x] < d[w]) w = x;
		}

		// check if graph is disconnected
		if (d[w] == INT_MAX)
		{
			cerr << "distance is max, disconnected graph for " << w << endl;
			break;
		}
		if (vflag)
			cerr << "w = " << w << endl;

		// add node to S and remove from V
		InsertIntoSet(s, w);
		DeleteFromSet(v, w);

		// verify output
		if (vflag)
		{
			DumpSet("5, S = ", s);
			DumpSet("5, V = ", v);
		}

		// update distances
		for (x = FindFirstMember(v, vsave);
		     x;
		     x = FindNextMember(v, vsave))
		{
			// update distances
			if (d[x] > (d[w]+tables[Express][w][x].time))
			{
				d[x] = d[w]+tables[Express][w][x].time;
				path[x] = w;
			}
		}

		// verify output
		if (vflag)
		{
			DumpSet("10, D = ", d);
			DumpSet("10, PATH = ", path);
		}
	}

	// print out results
	cout << endl << "path array ... " << endl;
	if (vflag)
		cerr << endl << "path array ... " << endl;
	for (i = 1; i <= nf; i++)
	{
		cout << "path[" << i << "] = " << path[i] << endl;
		if (vflag)
			cerr << "path[" << i << "] = " << path[i] << endl;
	}

	// elevators to take
	i = f2;
	cout << endl << "floor is ... " << f2 << endl;
	while (path[i] != 0)
	{
		cout << "floor is ... " << path[i] << endl;
		i = path[i];
	}

	// all done
	return;
}

// main entry point
main(int argc, char **argv)
{
	// get options
	for (int c = 0; (c = getopt(argc, argv, "?v")) != EOF; )
	{
		switch (c)
		{
		case 'v':
			vflag = 1;
			break;
		case '?':
			cerr << "usage: " << argv[0] << " [-?v] [infile]" << endl;
			return(2);

		default:
			cerr << "invalid option." << endl;
			cerr << "usage: " << argv[0] << " [-?v] [infile]" << endl;
			return(2);
		}
	}

	// get data about floors
	int floors;
	char *fname = ((argc >= 2) ? argv[optind] : NULL);
	assert(GetFloorData(fname, floors) == 0);

	// find direct paths for now
	int f1, f2;
	cout << "enter start floor: ";
	cin >> f1;
	while (f1 > 0 && cin && ! cin.eof())
	{
		// get destination floor
		cout << "enter destination floor: ";
		cin >> f2;
		if (f2 <= 0 || ! cin) break;

		// get time and path from floor1 to floor2
		GetLeastTimePath(floors, f1, f2);

		// get the next case
		cout << "enter start floor: ";
		cin >> f1;
	}

	// all done
	return(0);
}

