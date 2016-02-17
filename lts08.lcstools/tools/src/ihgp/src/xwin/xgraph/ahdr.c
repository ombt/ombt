// add header for graphing

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
	cerr << "\tadd header for plotting." << endl;
	return;
}

// main entry point
main(int argc, char **argv)
{
	// options
	double xlimit = -1.0;
	double ylimit = -1.0;

	// get comand line options
	for (int c = 0; (c = getopt(argc, argv, "?x:y:")) != EOF; )
	{
		switch (c)
		{
		case 'x':
			xlimit = strtod(optarg, NULL);
			break;

		case 'y':
			ylimit = strtod(optarg, NULL);
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

	// attach header info
	cout << StartOfOptions << endl;
	if (xlimit > 0)
	{
		cout << MinimumXValue << " " << -xlimit << endl;
		cout << MaximumXValue << " " << xlimit << endl;
	}
	if (ylimit > 0)
	{
		cout << MinimumYValue << " " << -ylimit << endl;
		cout << MaximumYValue << " " << ylimit << endl;
	}
	cout << XAxisPlotType << " LINEAR" << endl;
	cout << YAxisPlotType << " LINEAR" << endl;
	cout << StartOfData << endl;

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

