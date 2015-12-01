#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>

using namespace std;

void
usage(const char *cmd)
{
    cout << "usage: " << cmd << " max_prime" << endl;
}

int
main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
        return -1;
    }

    long long  max_prime = ::atoll(argv[1]);
    cout << "Max Prime: " << max_prime << endl;
    assert(max_prime > 0);

    char *primes = new char [max_prime+1];
    memset(primes, 1, max_prime);

    primes[0] = 0;
    primes[1] = 0;

    for (long long p=2; p<=max_prime; ++p)
    {
        if (primes[p] != 0)
        {
            for (long long c = 2*p; c<=max_prime; c += p)
            {
                primes[c] = 0;
            }
        }
    }

    for (long long p=0; p<=max_prime; ++p)
    {
        if (primes[p] == 1)
        {
            cout << "Prime: " << p << endl;
        }
    }

    return 0;
}
