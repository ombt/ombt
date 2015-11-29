// reflect about y or x axis.

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"

// externs
extern char *optarg;
extern int optind;

// usage message
void
usage(char *cmd)
{
	cerr << "usage: " << cmd << " " ;
	cerr << "[-HI] [-x] [-y]" << endl;
	cerr << endl;
	cerr << "where:" << endl;
	cerr << "\t-H - attach spectrum header," << endl;
	cerr << "\t-I - generate inverse operation," << endl;
	cerr << "\t-x - reflect about x-axis," << endl;
	cerr << "\t-y - reflect about y-axis." << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// variables for generating sine wave.
	int inverse = 0;
	int hdr = 0;
	double mx = 1.0;
	double my = 1.0;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?IHxy")) != EOF; )
	{
		switch (c)
		{
		case 'H':
			hdr = 1;
			break;

		case 'I':
			inverse = 1;
			break;

		case 'x':
			my = -1.0;
			break;

		case 'y':
			mx = -1.0;
			break;

		case '?':
			usage(argv[0]);
			return(0);

		default:
			ERROR("invalid option", EINVAL);
			usage(argv[0]);
			return(2);
		}
	}

	// attach header
	if (hdr)
	{
		cout << "++++ START OPTIONS ++++" << endl;
		cout << "++++ START DATA ++++" << endl;
	}

	// now read in data
	while ( ! cin.eof())
	{
		// read in x,y point
		double tmpx, tmpy;
		cin >> tmpx >> tmpy;

		// get new values
		double newx = my*tmpx;
		double newy = mx*tmpy;
	
		// write out new points
		cout << newx << " " << newy << endl;
	}

	// all done
	return(0);
}

