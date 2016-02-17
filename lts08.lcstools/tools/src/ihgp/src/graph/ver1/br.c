/* reverse bits in an integer */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>

char *
num2base(unsigned long base, unsigned long number)
{
	long ic, quotient, remainder;
	static char buf[64];

	ic = sizeof(buf);
	buf[--ic] = 0;
	do {
		quotient = number/base;
		remainder = number % base;
		if (remainder < 10)
		{
			buf[--ic] = '0' + (char)remainder;
		}
		else
		{
			buf[--ic] = 'A' + (char)remainder - 10;
		}
	} while ((number = quotient) != 0);
	return(buf+ic);
}

main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s bits number\n", argv[0]);
		exit(2);
	}
	long bits = atol(argv[1]);
	long input = atol(argv[2]);
	long output = 0;
	printf("input is %s.\n", num2base(2L, input));
	for (int ibits = 1; ibits <= bits; ibits++)
	{
		output = (output << 1) + (input & 1);
		input >>= 1;
	}
	printf("output is %s.\n", num2base(2L, output));
	exit(0);
}

