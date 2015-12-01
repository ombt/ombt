//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "adt/Array.h"
#include "adt/String.h"


using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	// check input
	MustBeTrue(argc > 1);

	Array<String> strings(argc);
	for (int arg=0; arg < argc; arg++)
	{
		strings[arg] = argv[arg];
	}

	ArrayIterator<String> iter(strings);
	for ( ; ! iter.done(); iter++)
	{
		cout << iter() << endl;
	}

	cout << "dump strings array: " << strings << endl;

	ArrayIterator_Reverse<String> riter(strings);
	for ( ; ! riter.done(); riter++)
	{
		cout << riter() << endl;
	}

	return(0);
}
