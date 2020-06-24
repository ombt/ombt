// traverse a DAG given a starting point

// unix headers
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

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

	// read in a list of files
	int ireccnt = 0;
	for (int iarg = optind; iarg < argc; argc++)
	{
		// which file is opened
		cout << "opening file ... " << argv[iarg] << endl;

		// open input file
		int infd = open(argv[iarg], O_RDONLY);
		if (infd == -1)
		{
			ERRORD("unable to open file", argv[iarg], errno);
			return(-2);
		}

		// get size of file
		struct stat sbuf;
		if (fstat(infd, &sbuf) == -1)
		{
			ERRORD("unable to stat file", argv[iarg], errno);
			return(-2);
		}

		// allocate buffer for reading in file
		Pointer<char> pbuf = new char [sbuf.st_size];
		MustBeTrue(pbuf != NULL);

		// read in entire file
		if (read(infd, (char *)pbuf, sbuf.st_size) != sbuf.st_size)
		{
			ERRORD("unable to read file", argv[iarg], errno);
			return(-2);
		}

		// close input file
		close(infd);
		infd = -1;

		// assign a string to buffer
		String data(pbuf);

		// tokenizer 
		StringTokens tokenizer(data, String(" \t\n"));

		// read data from file
		for ( ; !tokenizer.done(); ireccnt++, tokenizer++)
		{
			// read in dag link
			String node = tokenizer();
			tokenizer++;
			if (tokenizer.done()) 
				continue;
			String child = tokenizer();
			
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
	}

	// all done
	return(0);
}

