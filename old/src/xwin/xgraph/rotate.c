// rotate graph a given angle

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
	cerr << "[-DHI] [-p angle | -m angle] " << endl;
	cerr << endl;
	cerr << "where:" << endl;
	cerr << "\t-H - attach spectrum header," << endl;
	cerr << "\t-D - angles are in degrees," << endl;
	cerr << "\t-I - generate inverse operation," << endl;
	cerr << "\t-p - rotate +angle," << endl;
	cerr << "\t-m - rotate -angle." << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// variables for generating sine wave.
	int inverse = 0;
	int hdr = 0;
	double deg2rads = 1.0;
	double theta = 0.0;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?IHDp:m:")) != EOF; )
	{
		switch (c)
		{
		case 'H':
			hdr = 1;
			break;

		case 'I':
			inverse = 1;
			break;

		case 'D':
			deg2rads = M_PI/180.0;
			break;

		case 'p':
			theta = strtod(optarg, NULL);
			break;

		case 'm':
			theta = -1.0*strtod(optarg, NULL);
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
		theta = -1.0*theta;

	// now read in data
	while ( ! cin.eof())
	{
		// read in x,y point
		double tmpx, tmpy;
		cin >> tmpx >> tmpy;

		// get new angle
		double newx = 
			tmpx*cos(theta*deg2rads) - tmpy*sin(theta*deg2rads);
		double newy = 
			tmpx*sin(theta*deg2rads) + tmpy*cos(theta*deg2rads);
	
		// write out new points
		cout << newx << " " << newy << endl;
	}

	// all done
	return(0);
}

