#include <stdio.h>
#include <string.h>
#include <libgen.h>

main(int argc, char **argv)
{
	// const char *intre = "^[ \t]*([+-]{0,1}[0-9]+)$0";
	// const char *realre = "^[ \t]*([-+]{0,1}[0-9]+(\\.[0-9]*){0,1}([eEqQ][-+]{0,1}[0-9]+){0,1}|[-+]{0,1}INF|QNAN|SNAN)$0";
	const char *realre[4];
	realre[0] = "^[ \t]*([-+]{0,1}[0-9]+(\\.[0-9]*){0,1}([eEqQ][-+]{0,1}[0-9]+){0,1})$0";
	realre[1] = "^[ \t]*([-+]{0,1}INF)$0";
	realre[2] = "^[ \t]*(QNAN)$0";
	realre[3] = "^[ \t]*(SNAN)$0";
	const char *crealre[4];

	for (int i=0; i<4; i++)
	{
		if ((crealre[i] = regcmp(realre[i], NULL)) == NULL)
			printf("regcmp failed ...\n");
		else
			printf("regcmp %d succeeded ...\n", i);
	}

	for (int arg=1; arg<argc; arg++)
	{
		char *cursor = argv[arg];
		char *newcursor = NULL;
		for (int re=0; re<4; re++)
		{
			printf("scanning ... %s with RE %d\n", cursor, re);
			char realval[BUFSIZ];
			for (int i=1; 
			    ((newcursor = regex(crealre[re], cursor, realval)) != NULL);
			      i++)
			{
				printf("%d: argv[%d]=%s, realval=<%s>\n", 
					i, arg, argv[arg], realval);
				cursor=newcursor;
			}
		}
	}

	return(0);
}

