// miscellaneous routines

// headers
#include "miscutils.h"

// convert a string to an integer
String
itoa(int i)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d", i);
	return(String(buf));
}

// make a directory
int
makedir(const String &d)
{
	if (access((const char *)d, R_OK|W_OK|X_OK) != OK)
	{
		// check if directory exists
		if (errno == ENOENT)
		{
			if (mkdir((const char *)d, 0755) != OK)
			{
				ERRORD("mkdir failed.", d, errno);
				return(NOTOK);
			}
		}
		else
		{
			ERRORD("access failed.", d, errno);
			return(NOTOK);
		}
	}
	return(OK);
}
