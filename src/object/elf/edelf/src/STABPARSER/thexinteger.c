#include <stdio.h>
#include <string.h>
#include <libgen.h>

main(int argc, char **argv)
{
	const char *hexintre = "^[ \t]*([A-F0-9]+)$0";
	const char *chexintre = NULL;

	if ((chexintre = regcmp(hexintre, NULL)) == NULL)
		printf("regcmp failed ...\n");
	else
		printf("regcmp succeeded ...\n");

	for (int arg=1; arg<argc; arg++)
	{
		char *cursor = argv[arg];
		char *newcursor = NULL;
		char hexintval[BUFSIZ];
		for (int i=1; 
		    ((newcursor = regex(chexintre, cursor, hexintval)) != NULL); i++)
		{
			printf("%d: argv[%d]=%s, hexintval=<%s>\n", 
				i, arg, argv[arg], hexintval);
			cursor = newcursor;
		}
	}

	return(0);
}
