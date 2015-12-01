//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdlib.h>
#include <iostream>
#include "adt/String.h"
#include "adt/Stack_List.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	int arg;
	
	// size of stack
	Stack_List<String> stack;

	// insert data items
	for (arg = 2; arg < argc; arg++)
	{
		MustBeTrue(stack.push(String(argv[arg])) == OK);
	}
	cout << "stack after inserts is ... " << stack << endl;
	while (!stack.isEmpty())
	{
		String tmp;
		MustBeTrue(stack.pop(tmp) == OK);
		cout << "popped item is ... " << tmp << endl;
	}
	cout << "stack after deletes is ... " << stack << endl;

	return(0);
}
