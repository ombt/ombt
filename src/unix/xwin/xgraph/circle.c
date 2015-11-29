// generate a circle

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
	cerr << "[-DH] [-s angle -e angle] [-d delta angle] [-r radius] " << endl;
	cerr << endl;
	cerr << "where:" << endl;
	cerr << "\t-H - attach spectrum header," << endl;
	cerr << "\t-D - angles are in degrees," << endl;
	cerr << "\t-s - start angle," << endl;
	cerr << "\t-e - end angle," << endl;
	cerr << "\t-d - delta angle for step size," << endl;
	cerr << "\t-r - radius of circle." << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// variables for generating sine wave.
	int hdr = 0;
	double deg2rads = 1.0;
	double radius = 1.0;
	double dtheta = 0.01;
	double start = 0.0;
	double end = 2*M_PI;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?HDd:s:e:r:")) != EOF; )
	{
		switch (c)
		{
		case 'H':
			hdr = 1;
			break;

		case 'D':
			deg2rads = M_PI/180.0;
			break;

		case 'd':
			dtheta = strtod(optarg, NULL);
			break;

		case 's':
			start = strtod(optarg, NULL);
			break;

		case 'e':
			end = strtod(optarg, NULL);
			break;

		case 'r':
			radius = strtod(optarg, NULL);
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

	// write sine function
	for (double angle = start; angle <= end; angle += dtheta)
	{ 
		double x = radius*cos(angle*deg2rads);
		double y = radius*sin(angle*deg2rads);
		cout << x << " " << y << endl;
	}

	// all done
	return(0);
}

