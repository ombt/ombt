// 
// stable marriage problem. you are given a group of N-men and N-women.
// each man lists in order of preference each women. similary, each woman
// lists in order of preference each man.
//
// the problem is to find a set of marriages that are stable, that is,
// a man is not married to woman who he prefers less than another woman
// who prefers him over her current husband.
//
//////////////////////////////////////////////////////////////////////////////
//
// headers
#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <assert.h>
#include <errno.h>

// local headers
#include "debug.h"
#include "returns.h"

// constants
const int MaxNumberOfMen = 50;
const int MaxNumberOfWomen = 50;

// men's and women's list of preferences
int mlp[MaxNumberOfMen+1][MaxNumberOfWomen+1];
int wlp[MaxNumberOfWomen+1][MaxNumberOfMen+1];

// women's ranking of men
int wrm[MaxNumberOfWomen+1][MaxNumberOfMen+1];

// current fiancee for each women
int wf[MaxNumberOfWomen+1];

// current location in men's preference list
int mlpi[MaxNumberOfMen+1];

// number of men or women
int nmen, nwomen;

// read in data
int
ReadData(char *fname)
{
	// input stream
	filebuf *fbp = NULL;
	istream_withassign fin;

	// open and read file
	if (fname != NULL)
	{
		fbp = new filebuf;
		assert(fbp != NULL);
		if (fbp->open(fname, ios::in) == 0)
			return(NOTOK);
		fin = fbp;
	}
	else
		fin = cin.rdbuf();

	// get how many men and women
	cout << "enter number of men: " << endl;
	fin >> nmen;
	cout << "enter number of women: " << endl;
	fin >> nwomen;

	// read in men's list of preferences
	for (int m = 1; m <= nmen; m++)
	{
		cout << "enter preferences of man " << m << "." << endl;
		for (int w = 1; w <= nwomen; w++)
		{
			fin >> mlp[m][w];
		}
	}

	// initialize index list
	for (m = 1; m <= nmen; m++)
	{
		mlpi[m] = 1;	// start with first choice
	}

	// read in women's list of preferences
	for (int w = 1; w <= nwomen; w++)
	{
		cout << "enter preferences of woman " << w << "." << endl;
		for (m = 1; m <= nmen; m++)
		{
			fin >> wlp[w][m];
		}
	}

	// women's ranking of men.
	for (w = 1; w <= nwomen; w++)
	{
		for (m = 1; m <= nmen; m++)
		{
			wrm[w][wlp[w][m]] = m;
		}
	}

	// initialize fiancee list
	for (w = 1; w <= nwomen; w++)
	{
		wf[w] = -1;	// no fiancee as of now.
	}

	// print out data
	cerr << "men's list of preference ..." << endl;
	for (m = 1; m <= nmen; m++)
	{
		cerr << "man " << setw(3) << m << ": ";
		for (w = 1; w <= nwomen; w++)
		{
			cerr << setw(5) << mlp[m][w];
		}
		cerr << endl;
	}
	cerr << endl;
	cerr << "women's list of preference ..." << endl;
	for (w = 1; w <= nwomen; w++)
	{
		cerr << "woman " << setw(3) << w << ": ";
		for (m = 1; m <= nmen; m++)
		{
			cerr << setw(5) << wlp[w][m];
		}
		cerr << endl;
	}
	cerr << endl;
	cerr << "women's rankings of men ..." << endl;
	for (w = 1; w <= nwomen; w++)
	{
		cerr << "woman " << setw(3) << w << ": ";
		for (m = 1; m <= nmen; m++)
		{
			cerr << setw(5) << wrm[w][m];
		}
		cerr << endl;
	}
	cerr << endl;

	// all done
	if (fbp != NULL) delete fbp;
	return(OK);
}

// find a set of stable marriages
int
StableMarriages()
{
	// start pairings
	for (int m = 1; m <= nmen; m++)
	{
		// loop to do assignments
		for (int cm = m; ; )
		{
			// get next prefered female for current male
			int w = mlp[cm][mlpi[cm]++];

			// is this female taken?
			if (wf[w] == -1)
			{
				// no suitor, assign current male to her.
				wf[w] = cm;
				break;		// goto next male.
			}
			else if (wrm[w][cm] < wrm[w][wf[w]])
			{
				// she prefers current male to current
				// fiancee. 
				int cf = wf[w];
				wf[w] = cm;
				cm = cf;
			}
		}
	}

	// stable marriages are ...
	cout << "stable marriages (F,M) are ... " << endl;
	for (int w = 1; w <= nwomen; w++)
	{
		cout << "(" << w << "," << wf[w] << ")" << endl;
	}
	return(OK);
}

// start point
main(int argc, char **argv)
{
	// read in preference data
	if (ReadData(((argc >= 2) ? argv[1] : NULL)) != OK)
	{
		ERROR("ReadData failed", errno);
		return(2);
	}

	// get stable marriages
	if (StableMarriages() != OK)
	{
		ERROR("StableMarriages failed", errno);
		return(2);
	}

	// all done
	return(0);
}


