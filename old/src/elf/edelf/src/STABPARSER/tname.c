#include <stdio.h>
#include <string.h>
#include <libgen.h>

main(int argc, char **argv)
{
	const char *wsre = "^[ \t]*";
	const char *cwsre = NULL;
	if ((cwsre = regcmp(wsre, NULL)) == NULL)
		printf("regcmp failed ...\n");
	else
		printf("regcmp succeeded ...\n");

	const char *conre = "[^;:\'\"]*([;:\'\"])$0";
	const char *cconre = NULL;
	if ((cconre = regcmp(conre, NULL)) == NULL)
		printf("regcmp failed ...\n");
	else
		printf("regcmp succeeded ...\n");

	for (int arg=1; arg<argc; arg++)
	{
		char *cursor = argv[arg];
		char *newcursor = NULL;
		char strval[BUFSIZ];

		// check for white space
		if ((newcursor = regex(cwsre, cursor)) != NULL)
		{
			// skip white space
			cursor = newcursor;
		}

		// look for a name
		char *startname = cursor;
		char *endname = cursor;
		int found = 0;
		for (int done=0; !done && 
		    (newcursor = regex(cconre, cursor, strval)) != NULL; )
		{
			if (*strval == ':' && *newcursor == ':')
			{
				// double colons are allowed in names.
				cursor = newcursor+1;
			}
			else
			{
				// we found the end
				endname = newcursor-1;
				strncpy(strval, startname, endname-startname);
				strval[endname-startname] = '\0';
				printf("FOUND A NAME ... <%s>\n", 
					strval);
				found = done = 1;
				continue;
			}
		}
		if (!found)
			printf("NOT A NAME ... %s\n", startname);
	}
	return(0);
}
