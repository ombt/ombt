#include <stdio.h>
#include <string.h>
#include <libgen.h>

main(int argc, char **argv)
{
	const char *intre = "^[ \t]*([+-]{0,1}[0-9]+)$0";
	const char *cintre = NULL;

	if ((cintre = regcmp(intre, NULL)) == NULL)
		printf("regcmp failed ...\n");
	else
		printf("regcmp succeeded ...\n");

	for (int arg=1; arg<argc; arg++)
	{
		char *cursor = argv[arg];
		char *newcursor = NULL;
		char intval[BUFSIZ];
		for (int i=1; 
		    ((newcursor = regex(cintre, cursor, intval)) != NULL); i++)
		{
			printf("%d: argv[%d]=%s, intval=<%s>\n", 
				i, arg, argv[arg], intval);
			cursor = newcursor;
		}
	}

	return(0);
}
