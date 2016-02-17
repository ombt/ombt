// scale graph by a given constant

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
	cerr << "[-HI] [-x factor] [-y factor] " << endl;
	cerr << endl;
	cerr << "where:" << endl;
	cerr << "\t-H - attach spectrum header," << endl;
	cerr << "\t-I - generate inverse operation," << endl;
	cerr << "\t-x - scale x-axis by factor," << endl;
	cerr << "\t-y - scale y-axis by factor." << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// variables for generating sine wave.
	int inverse = 0;
	int hdr = 0;
	double xscale = 1.0;
	double yscale = 1.0;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?IHx:y:")) != EOF; )
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
			xscale = strtod(optarg, NULL);
			break;

		case 'y':
			yscale = strtod(optarg, NULL);
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

	// check for inverse
	if (inverse)
	{
		MustBeTrue(xscale != 0.0 && yscale != 0.0);
		xscale = 1.0/xscale;
		yscale = 1.0/yscale;
	}

	// now read in data
	while ( ! cin.eof())
	{
		// read in x,y point
		double tmpx, tmpy;
		cin >> tmpx >> tmpy;

		// scale x, y values
		double newx = xscale*tmpx;
		double newy = yscale*tmpy;
	
		// write out new points
		cout << newx << " " << newy << endl;
	}

	// all done
	return(0);
}

