//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "adt/String.h"
#include "adt/List.h"

using namespace ombt;
using namespace std;

// ostream &operator<<(ostream &, const List<String> &);

main(int argc, char **argv)
{
	int arg;
	// tests for unordered list operations
	List<String> unordered;
	for (arg = 1; arg < argc; arg++)
	{
		unordered.insert(String(argv[arg]));
	}
	ListIterator<String> unorderedIter(unordered);
	for ( ; !unorderedIter.done(); unorderedIter++)
	{
		cout << "unordered item is ... ";
		cout << unorderedIter() << endl;
	}
	for (arg = 1; arg < argc; arg++)
	{
		String tmp(argv[arg]);
		cout << "unordered list before is ... " << unordered << endl;
		unordered.remove(tmp);
		cout << "unordered deleted item is ... " << tmp << endl;
		cout << "unordered list after is ... " << unordered << endl;
	}
	cout << "final unordered list is ... " << unordered << endl;

	// tests for at-front list operations
	List<String> atfront;
	for (arg = 1; arg < argc; arg++)
	{
		atfront.insertAtFront(String(argv[arg]));
	}
	ListIterator<String> atfrontIter(atfront);
	for ( ; !atfrontIter.done(); atfrontIter++)
	{
		cout << "atfront item is ... ";
		cout << atfrontIter() << endl;
	}
	for (arg = 1; arg < argc; arg++)
	{
		String tmp(argv[arg]);
		cout << "atfront list before is ... " << atfront << endl;
		atfront.removeAtFront(tmp);
		cout << "atfront deleted item is ... " << tmp << endl;
		cout << "atfront list after is ... " << atfront << endl;
	}
	cout << "final at-front list is ... " << atfront << endl;

	// tests for at-end list operations
	List<String> atend;
	for (arg = 1; arg < argc; arg++)
	{
		atend.insertAtEnd(String(argv[arg]));
	}
	ListIterator<String> atendIter(atend);
	for ( ; !atendIter.done(); atendIter++)
	{
		cout << "atend item is ... ";
		cout << atendIter() << endl;
	}
	for (arg = 1; arg < argc; arg++)
	{
		String tmp(argv[arg]);
		cout << "atend list before is ... " << atend << endl;
		atend.removeAtEnd(tmp);
		cout << "atend deleted item is ... " << tmp << endl;
		cout << "atend list after is ... " << atend << endl;
	}
	cout << "final at-end list is ... " << atend << endl;

	// tests for ordered list operations with duplicates
	List<String> ordered;
	for (arg = 1; arg < argc; arg++)
	{
		ordered.insertOrdered(String(argv[arg]));
		ordered.insertOrdered(String(argv[arg]));
	}
	ListIterator<String> orderedIter(ordered);
	for ( ; !orderedIter.done(); orderedIter++)
	{
		cout << "ordered item is ... ";
		cout << orderedIter() << endl;
	}
	for (arg = 1; arg < argc; arg++)
	{
		String tmp(argv[arg]);
		cout << "ordered list before is ... " << ordered << endl;
		ordered.removeOrdered(tmp);
		cout << "ordered deleted item is ... " << tmp << endl;
		cout << "ordered list after is ... " << ordered << endl;
	}
	cout << "final ordered list wth duplicates is ... " << ordered << endl;

	// tests for ordered list operations without duplicates
	List<String> noduplicates;
	for (arg = 1; arg < argc; arg++)
	{
		noduplicates.insertOrderedUnique(String(argv[arg]));
		noduplicates.insertOrderedUnique(String(argv[arg]));
	}
	ListIterator<String> noduplicatesIter(noduplicates);
	for ( ; !noduplicatesIter.done(); noduplicatesIter++)
	{
		cout << "noduplicates item is ... ";
		cout << noduplicatesIter() << endl;
	}
	for (arg = 1; arg < argc; arg++)
	{
		String tmp(argv[arg]);
		cout << "noduplicates list before is ... " << noduplicates << endl;
		noduplicates.removeOrderedUnique(tmp);
		cout << "noduplicates deleted item is ... " << tmp << endl;
		cout << "noduplicates list after is ... " << noduplicates << endl;
	}
	cout << "final ordered list without duplicated is ... " << noduplicates << endl;

	return(0);
}
