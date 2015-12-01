//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// test for alphabet class

#include "random/Random.h"
#include "adt/Alphabet.h"
#include "adt/Array.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	MustBeTrue(argc == 2);

	// get the seed for a random number generator
	setKey(atol(argv[1]));
	
	// create an alphabet list
	List<char> ablist;
	for (int c = 'a'; c <= 'z'; c++)
	{
		ablist.insertAtEnd(c);
	}
	for (int c = 'A'; c <= 'Z'; c++)
	{
		ablist.insertAtEnd(c);
	}

	// create an Alphabet object
	Alphabet<char> abset(ablist);
DUMP(abset);

	// check comparisons
	if (abset.eq('a', 'a'))
		cout << "a == a is true !!!" << endl;
	else
		cout << "OOPS a == a is NOT true !!!" << endl;
	if (abset.neq('a', 'a'))
		cout << "OOPS a != a is NOT FALSE !!!" << endl;
	else
		cout << "a != a is false !!!" << endl;

	// create a random number list
	Array<unsigned long> ularray(1, 10);
	for (int c = 1; c <= 10; c++)
	{
		ularray[c] = random();
	}
	List<unsigned long> ullist;
	ArrayIterator<unsigned long> ularrayIter(ularray);
	for ( ; !ularrayIter.done(); ularrayIter++)
	{
		ullist.insertAtEnd(ularrayIter());
	}

	// create an number object
	Alphabet<unsigned long> ulset(ullist);
DUMP(ulset);

	// check comparisons
	if (ulset.eq(ularray[1], ularray[1]))
		cout << "ula[1] == ula[1] is true !!!" << endl;
	else
		cout << "OOPS ula[1] == ula[1] is NOT true !!!" << endl;
	if (ulset.neq(ularray[1], ularray[1]))
		cout << "OOPS ula[1] != ula[1] is NOT FALSE !!!" << endl;
	else
		cout << "ula[1] != ula[1] is false !!!" << endl;

	// check all operations
	for (ularrayIter.reset(); !ularrayIter.done(); ularrayIter++)
	{
		// get two letters in the alphabet
		unsigned long a = ularrayIter();
		if (ularrayIter.done())
			break;
		else
			ularrayIter++;
		unsigned long b = ularrayIter();

		// do all types of comparisons
DUMP(a);
DUMP(b);
		MustBeTrue(ulset.eq(a, a) != 0);
		MustBeTrue(ulset.eq(b, b) != 0);
		MustBeTrue(ulset.eq(a, b) == 0);
		MustBeTrue(ulset.lt(a, b) != 0);
		MustBeTrue(ulset.gt(a, b) == 0);
		MustBeTrue(ulset.ltoreq(a, b) != 0);
		MustBeTrue(ulset.gtoreq(a, b) == 0);
	}
	
	// check iterator
	cout << "iterator the first time ..." << endl;
	AlphabetIterator<char> absetIter(abset);
	for ( ; !absetIter.done(); absetIter++)
	{
		cout << "letter is ... " << absetIter() << endl;
	}
	cout << "iterator the second time ..." << endl;
	for (absetIter.reset(); !absetIter.done(); absetIter++)
	{
		cout << "letter is ... " << absetIter() << endl;
	}

	// test out word class
	Word<char> hello(abset, strlen("Hello"), "Hello");
	Word<char> blank(abset, strlen(" "), " ");
	Word<char> world(abset, strlen("World"), "World");

	// print word out
	hello = hello + blank + world;
	cout << "WORD IS ... " << hello << endl;

	// good bye world message
	Word<char> goodbye(abset, strlen("Good Bye"), "Good Bye");

	// use substrings 
	goodbye = goodbye + hello(5);
	cout << "WORD IS ... " << goodbye << endl;

	// use a word iterator
	cout << "(iterator) Word is ... ";
	WordIterator<char> goodbyeIter(goodbye);
	for ( ; !goodbyeIter.done(); goodbyeIter++)
	{
		cout << goodbyeIter();
	}
	cout << endl;

	// all done
	return(0);
}
