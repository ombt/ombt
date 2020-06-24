// headers
#include "myxlib.h"

// decode a key-press event
int
DecodeKeyPress(XKeyEvent *event, KeySym *keysym, char *string)
{
#if 0
	XComposeStatus composestatus;
#endif
	int length;

	// initialize
	*keysym = 0;
	*string = 0;

	// verify the event
	if ((event->type != KeyPress) && (event->type == KeyRelease))
	{
		return(False);
	}

	// convert event to a string
#if 0
	length =  XLookupString(event, string, 1, keysym, &composestatus);
#else
	length =  XLookupString(event, string, 1, keysym, NULL);
#endif
	if (length > 0)
		string[1] = 0;
	else
		string[0] = 0;
	
	// all done
	return(IsMetaQ(event, string));
}

// check for alternate keys
int
IsMetaKey(XKeyEvent *event)
{
	if ((event->type == KeyPress) || (event->type == KeyRelease))
	{
		if (event->state & Mod4Mask)
		{
			return(True);
		}
	}
	return(False);
}

// check for quit
int
IsMetaQ(XKeyEvent *event, char *string)
{
	int status;

	status = False;
	if (IsMetaKey(event) == True)
	{
		if (*string == 'Q' || *string == 'q')
		{
			status = True;
		}
	}
	return(status);
}
