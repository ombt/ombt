// generate a cosine curve

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
	cerr << "[-DH] [-s angle -e angle] [-d delta angle] [-a value] [-k value] [-p value] " << endl;
	cerr << endl;
	cerr << "where:" << endl;
	cerr << "\t-H - attach spectrum header," << endl;
	cerr << "\t-D - angles are in degrees," << endl;
	cerr << "\t-s - start angle," << endl;
	cerr << "\t-e - end angle," << endl;
	cerr << "\t-d - delta angle for step size," << endl;
	cerr << "\t-a - value of A in A*sin(K*x+Phi)," << endl;
	cerr << "\t-k - value of K in A*sin(K*x+Phi)," << endl;
	cerr << "\t-p - value of Phi in A*sin(K*x+Phi)," << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// variables for generating sine wave.
	int hdr = 0;
	double deg2rads = 1.0;
	double a = 1.0;
	double k = 1.0;
	double dtheta = 0.01;
	double phi = 0.0;
	double start = -M_PI;
	double end = M_PI;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?HDd:s:e:a:k:t:")) != EOF; )
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

		case 'a':
			a = strtod(optarg, NULL);
			break;

		case 'k':
			k = strtod(optarg, NULL);
			break;

		case 'p':
			phi = strtod(optarg, NULL);
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

	// write cosine function
	for (double angle = start; angle <= end; angle += dtheta)
	{ 
		cout << angle << " " << a*cos((k*angle+phi)*deg2rads) << endl;
	}

	// all done
	return(0);
}

