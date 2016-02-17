#include <stdio.h>
#include <stdlib.h>
#include <math.h>

main(int argc, char **argv)
{
	int prod1 = 1;
	int prod2 = 1;
	int modulo = atoi(argv[1]);
	for (int arg = 2; arg < argc; arg++)
	{
		prod1 *= atoi(argv[arg])%modulo;
		prod2 *= atoi(argv[arg]);
		printf("1: prod1 = %d\n", prod1);
		printf("1: prod2 = %d\n", prod2);
	}
	prod1 %= modulo;
	prod2 %= modulo;
	printf("2: prod1 = %d\n", prod1);
	printf("2: prod2 = %d\n", prod2);
	return(0);
}
