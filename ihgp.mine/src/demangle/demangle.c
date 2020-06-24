#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <demangle.h>

main(int argc, char ** argv)
{
	char symbol[BUFSIZ+1];
	char prototype[BUFSIZ+1];
	size_t size = BUFSIZ;

	if (argc > 1)
	{
		if (strcmp(argv[1], "-?") == 0)
		{
			fprintf(stdout, "usage: %s [-?] [[mangle_name] ... ]\n\n",
				argv[0]);
			fprintf(stdout, "reads in stdin, if no names are given.\n");
			return(0);
		}
		for (int arg = 1; arg < argc; arg++)
		{
			prototype[0] = 0;
			strcpy(symbol, argv[arg]);
			if (cplus_demangle(symbol, prototype, size) == 0)
			{
				fprintf(stdout, "symbol = %s\n", symbol);
				fprintf(stdout, "prototype = %s\n", prototype);
			}
			else
			{
				fprintf(stdout, 
					"demangle failed for symbol %s\n", 
					symbol);
			}
		}
	}
	else
	{
		while (fgets(symbol, BUFSIZ, stdin) != NULL)
		{
			prototype[0] = 0;
			symbol[strlen(symbol)-1] = 0;
			if (cplus_demangle(symbol, prototype, size) == 0)
			{
				fprintf(stdout, "symbol = %s\n", symbol);
				fprintf(stdout, "prototype = %s\n", prototype);
			}
			else
			{
				fprintf(stdout, 
					"demangle failed for symbol %s\n", 
					symbol);
			}
		}
	}
	return(0);
}
