#include "tokenize.h"

main(int argc, char **argv)
{
	MustBeTrue(argc == 3);

	tokenize(argv[1], argv[2]);

	for (int it=1; gettoken(it) != NULL; it++)
	{
		printf("token %d is <%s>\n", it, gettoken(it));
	}

	return(0);
}
