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

	const char *qmre = "([\"'])$0";
	const char *cqmre = NULL;
	if ((cqmre = regcmp(qmre, NULL)) == NULL)
		printf("regcmp failed ...\n");
	else
		printf("regcmp succeeded ...\n");
	
	const char *conre = "[^\\\"']*([\\\"'])$0";
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
		char qm[BUFSIZ];

		// check for white space
		if ((newcursor = regex(cwsre, cursor)) != NULL)
		{
			// skip white space
			cursor = newcursor;
		}

		// check for quotation marks, " or '
		if ((newcursor = regex(cqmre, cursor, qm)) == NULL)
		{
			printf("NOT A STRING ... %s\n", cursor);
			continue;
		}
		char *startqm = cursor;
		char *endqm = cursor;
		cursor = newcursor;
		printf("found a >>%s<< ...\n", qm);
		int found = 0;
		for (int done=0; !done && 
		    (newcursor = regex(cconre, cursor, strval)) != NULL; )
		{
			switch (*strval)
			{
			case '\\':
				// check for end-of-string
				if (*newcursor == '\0')
				{
					printf("NOT A STRING ... %s\n", 
						startqm);
					done = 1;
					continue;
				}
				// skip whatever was escaped, 
				cursor = ++newcursor;
				break;
			case '"':
			case '\'':
				if (*strval == *startqm)
				{
					// we found the end
					endqm = newcursor;
					strncpy(strval, startqm, endqm-startqm);
					strval[endqm-startqm] = '\0';
					printf("FOUND A STRING ... <%s>\n", 
						strval);
					found = done = 1;
					continue;
				}
				cursor = newcursor;
				break;
			}
		}
		if (!found)
			printf("NOT A STRING ... %s\n", startqm);
	}
	return(0);
}
