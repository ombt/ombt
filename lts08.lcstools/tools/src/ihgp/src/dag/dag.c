// traverse a DAG given a starting point

// unix headers
#include <sysent.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "queue_List.h"
#include "stack_List.h"
#include "map.h"
#include "pointer.h"
#include "mystring.h"

// externs
extern int optind;
extern char *optarg;

// dag
Map<String, Pointer<List<String> > > dag;

// main entry point
main(int argc, char **argv)
{
	String start("");

	// get command line options
	int c;
	while ((c = getopt(argc, argv, "?s:")) != EOF)
	{
		// get option
		switch (c)
		{
		case 's':
			start = String(optarg);
			break;

		default:
			ERROR("invalid option", EINVAL);
			return(-2);
		}
	}

	// read in list of nodes and children
	if (optind >= argc)
	{
		// read in stdin
		for (int ireccnt = 0; !cin.eof(); ireccnt++)
		{
			// read in dag link
			String node, child;
			cin >> node >> child;
#if 0
			cout << "node is ... " << node << endl;
			cout << "child is ... " << child << endl;
#endif

			// store in dag
			if (!dag.isInMap(node))
			{
				Pointer<List<String> > plist = new List<String>;
				MustBeTrue(plist != NULL);
				dag[node] = plist;
			}
			dag[node]->insert(child);

			// give a progress message
			if (ireccnt%1000 == 0 && ireccnt > 0)
				cout << ireccnt << " records read ..." << endl;
		}
	}
	else
	{
		// read in a list of files
		int ireccnt = 0;
		for (int iarg = optind; iarg < argc; iarg++)
		{
			// which file is opened
			cout << "opening file ... " << argv[iarg] << endl;

			// open input file
			ifstream fin;

			// open file for read 
			fin.open(argv[iarg]);
			if (!fin)
			{
				ERRORD("unable to open file", 
					argv[iarg], errno);
				return(-2);
			}

			// read data from file
			for ( ; !fin.eof(); ireccnt++)
			{
				// read in dag link
				String node, child;
				fin >> node >> child;
#if 0
				cout << "node is ... " << node << endl;
				cout << "child is ... " << child << endl;
#endif

				// store in dag
				if (!dag.isInMap(node))
				{
					Pointer<List<String> > plist = 
						new List<String>;
					MustBeTrue(plist != NULL);
					dag[node] = plist;
				}
				dag[node]->insert(child);

				// give a progress message
				if (ireccnt%1000 == 0 && ireccnt > 0)
					cout << ireccnt << " records read ..." << endl;
			}

			// close file
			fin.close();
		}
	}

	// all done
	return(0);
}

