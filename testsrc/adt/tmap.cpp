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

// extern ostream & ::operator<<(ostream &, const Map<int, int> &);

main(int argc, char **argv)
{
	// check input
	MustBeTrue(argc > 1);

	// sieve of Erast... to generate primes.
	int maxprime = atoi(argv[1]);
	MustBeTrue(maxprime > 1);

	// initialize prime array to all primes
	Map<int, int> primes;
	int ip;
	for (ip=1; ip <= maxprime; ip++)
	{
		primes[ip] = 1;
	}

	// remove composite numbers
	primes[1] = 0;
	for (ip = 2; ip <= maxprime; ip++)
	{
		if (primes[ip])
		{
			cout << "PRIME NUMBER ... " << ip << endl;
			for (int ip2 = 2*ip; ip2 <= maxprime; ip2 += ip)
			{
				primes[ip2] = 0;
			}
		}
	}

	// user iterator to print prime numbers
	MapIterator<int, int> iter(primes);
	for ( ; ! iter.done(); iter++)
	{
		if (iter.data())
			cout << "(USE ITERATOR) PRIME NUMBER ... " << iter.key() << endl;
	}

	// print prime numbers
	Map<int, int> *pprimes = &primes;
	cout << "primes are ... " << *pprimes << endl;

	// remove all numbers
	for (ip = 1; ip <= maxprime; ip++)
	{
		MustBeTrue(primes.remove(ip) == OK);
		cout << "maxprimes after delete of number ";
		cout << ip << " is ... " << *pprimes << endl;
	}
	return(0);
}




