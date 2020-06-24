// remove header from a graph

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"

// definitions for header
#define StartOfOptions "++++ START OPTIONS ++++"
#define StartOfData "++++ START DATA ++++"
#define MinimumXValue "MINIMUM X VALUE"
#define MaximumXValue "MAXIMUM X VALUE"
#define MinimumYValue "MINIMUM Y VALUE"
#define MaximumYValue "MAXIMUM Y VALUE"
#define XAxisPlotType "X-AXIS PLOT TYPE"
#define YAxisPlotType "Y-AXIS PLOT TYPE"
#define XAxisLabel "X-AXIS LABEL"
#define YAxisLabel "Y-AXIS LABEL"
#define GraphTitle "GRAPH TITLE"

// externs
extern char *optarg;
extern int optind;

// usage message
void
usage(char *cmd)
{
	cerr << "usage: " << cmd << endl;
	cerr << "\tremove header for spectrum." << endl;
	return;
}

// specialized string functions
int
mystrncmp(char *string1, char *string2)
{
	return(strncmp(string1, string2, strlen(string2)));
}

// main entry point
main(int argc, char **argv)
{
	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?")) != EOF; )
	{
		switch (c)
		{
		case '?':
			usage(argv[0]);
			return(0);

		default:
			ERROR("invalid option", EINVAL);
			usage(argv[0]);
			return(2);
		}
	}

	// remove header from stream
	char inbuf[BUFSIZ+1];
	for (cin.getline(inbuf, BUFSIZ); 
	     ! cin.eof(); 
	     cin.getline(inbuf, BUFSIZ))
	{
		if (mystrncmp(inbuf, StartOfData) == 0)
		{
			// break out of options loop
			break;
		}
	}

	// check if out of data
	if (cin.eof())
	{
		ERROR("unexpected EOF. no data read in.", EINVAL);
		return(NOTOK);
	}


	// just read stdin and write to stdout.
	while ( ! cin.eof())
	{
		// read in x,y point
		double tmpx, tmpy;
		cin >> tmpx >> tmpy;

		// write out new points
		cout << tmpx << " " << tmpy << endl;
	}

	// all done
	return(0);
}

