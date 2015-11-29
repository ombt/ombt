#include <adt/Prime.h>

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	assert(argc == 2);
	Prime primes(atoi(argv[1]));
	cout << "primes are ... " << endl;
	cout << primes;
	return(0);
}
