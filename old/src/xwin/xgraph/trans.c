// translate origin by a vector

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
	cerr << "[-HI] x y" << endl;
	cerr << endl;
	cerr << "where:" << endl;
	cerr << "\t-H - attach spectrum header," << endl;
	cerr << "\t-I - generate inverse operation," << endl;
	cerr << "\tx - x-coordinate for vector displacment," << endl;
	cerr << "\ty - y-coordinate for vector displacment." << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// variables for generating sine wave.
	int inverse = 0;
	int hdr = 0;
	double tx = 0.0;
	double ty = 0.0;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?IH")) != EOF; )
	{
		switch (c)
		{
		case 'H':
			hdr = 1;
			break;

		case 'I':
			inverse = 1;
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

	// check of x, y was given
	if ((argc-optind) < 2)
	{
		ERROR("x,y vector not given.", EINVAL);
		usage(argv[0]);
		return(2);
	}

	// get translations
	tx = strtod(argv[optind++], NULL);
	ty = strtod(argv[optind], NULL);

	// attach header
	if (hdr)
	{
		cout << "++++ START OPTIONS ++++" << endl;
		cout << "++++ START DATA ++++" << endl;
	}

	// inverse ?
	if (inverse)
	{
		tx = -1.0*tx;
		ty = -1.0*ty;
	}

	// now read in data
	while ( ! cin.eof())
	{
		// read in x,y point
		double tmpx, tmpy;
		cin >> tmpx >> tmpy;

		// get new values
		double newx = tx+tmpx;
		double newy = ty+tmpy;
	
		// write out new points
		cout << newx << " " << newy << endl;
	}

	// all done
	return(0);
}

