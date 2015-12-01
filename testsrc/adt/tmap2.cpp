//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "adt/Map.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	int imax = atoi(argv[1]);

	Map<int, int> *pmap = new Map<int, int>;
	for (int i = 1; i <= imax; i++)
	{
		(*pmap)[i] = i;
	}
	cerr << "map is ... " << *pmap << endl;

	Map<int, int> *pmap2 = new Map<int, int>(*pmap);
	cerr << "map2 is ... " << *pmap2 << endl;

	cerr << "deleting map ... " << endl;
	delete pmap;
	cerr << "map deleted ... " << endl;

	cerr << "map2 is ... " << *pmap2 << endl;

	cerr << "deleting map2 ... " << endl;
	delete pmap2;
	cerr << "map2 deleted ... " << endl;

	return(0);
}



