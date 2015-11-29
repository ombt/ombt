// headers
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <adt/Array.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	// check input
	MustBeTrue(argc > 1);

	// sieve of Erast... to generate primes.
	int maxprime = atoi(argv[1]);
	MustBeTrue(maxprime > 1);

	// initialize prime array to all primes
	Array<char> primes(1, maxprime);
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
	ArrayIterator<char> iter(primes);
	for ( ; ! iter.done(); iter++)
	{
		if (iter())
			cout << "PRIME NUMBER ... " << iter.getIndex() << endl;
	}
	ArrayIterator_Reverse<char> riter(primes);
	for ( ; ! riter.done(); riter++)
	{
		if (riter())
			cout << "PRIME NUMBER ... " << riter.getIndex() << endl;
	}

	// reset size to half of current value
	primes.setBounds(1,maxprime/2);
	for (iter.reset(); ! iter.done(); iter++)
	{
		if (iter())
			cout << "PRIME NUMBER ... " << iter.getIndex() << endl;
	}
	for (riter.reset(); ! riter.done(); riter++)
	{
		if (riter())
			cout << "PRIME NUMBER ... " << riter.getIndex() << endl;
	}

	// print out sizes
	cout << "SIZE IS ... " << primes.getSize() << endl;
	cout << "LOWER IS ... " << primes.getLower() << endl;
	cout << "UPPER IS ... " << primes.getUpper() << endl;

	return(0);
}
