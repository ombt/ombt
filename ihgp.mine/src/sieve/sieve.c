/* sieve of Eratosthenes (sp?) */
#include <stdio.h>

/* externs */
extern int optind;
extern char *optarg;

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	long prime, notprime, maxprime;
	char *primes;

	/* check that a number was given */
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s maximum_number\n", argv[0]);
		exit(2);
	}
	maxprime = atol(argv[1]) + 1;
	if (maxprime < 2)
	{
		fprintf(stderr, "maximum prime must be greater than 2.\n");
		exit(2);
	}

	/* allocate and initialize array */
	primes = (char *)malloc(maxprime);
	if (primes == (char *)0)
	{
		fprintf(stderr, "unable to malloc prime array.\n");
		exit(2);
	}
	for (prime = 0; prime <= maxprime; prime++)
	{
		primes[prime] = 1;
	}
	primes[0] = 0;
	primes[1] = 0;

	/* start sieve processing */
	for (prime = 0; prime < maxprime; prime++)
	{
		/* check if the number is prime */
		if ( ! primes[prime]) continue;

		/* print prime number */
		fprintf(stdout, "%d\n", prime);

		/* a prime, turn off multiples */
		for (notprime = 2*prime; notprime < maxprime;
		     notprime += prime)
		{
			primes[notprime] = 0;
		}
	}

	/* all done */
	exit(0);
}

