// federal tax program

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "db.h"

// externs
extern int optind, errno;
extern char *optarg;

// main entry point
main(int argc, char **argv)
{
	// default values
	char dbfile[BUFSIZ];
	strcpy(dbfile, "db.in");

	// check command line arguments
	for (int c = 0; (c = getopt(argc, argv, "?d:")) != EOF; )
	{
		switch (c)
		{
		case 'd':
			// get database name
			strcpy(dbfile, optarg);
			break;

		case '?':
			// usage message
			cerr << "usage: tdb [-?] [-d database] class query";
			cerr << endl;
			return(2);

		default:
			// error
			cerr << "invalid options. use -? for help";
			cerr << endl;
			return(2);
		}
	}

	// class and query
	if ((optind + 2) > argc)
	{
		cerr << "class or query is missing.";
		cerr << endl;
		cerr << "usage: tdb [-?] [-d database] class query";
		cerr << endl;
		return(2);
	}
	char myclass[BUFSIZ];
	strcpy(myclass, argv[optind++]);
	char myquery[BUFSIZ];
	strcpy(myquery, argv[optind++]);

	// initialize the database
	Database db(dbfile);

	// issue query
	char response[BUFSIZ];
	if (db.query(myclass, myquery, response) != OK)
	{
		cerr << "query failed." << endl;
		return(2);
	}
	cout << "response is ... " << response << endl;

	// all done
	return(0);
}
