/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* decode a key-press event */
int
DecodeKeyPress(event, keysym, string)
XKeyEvent *event;
KeySym *keysym;
char *string;
{
	XComposeStatus composestatus;
	int length;

	/* initialize */
	*keysym = 0;
	*string = 0;

	/* verify the event */
	if ((event->type != KeyPress) && (event->type == KeyRelease))
	{
		return(False);
	}

	/* convert event to a string */
	length =  XLookupString(event, string, 1, keysym, &composestatus);
	if (length > 0)
		string[1] = 0;
	else
		string[0] = 0;
	
	/* all done */
	return(IsMetaQ(event, string));
}

/* check for meta keys */
int
IsMetaKey(event)
XEvent *event;
{
	if ((event->type == KeyPress) || (event->type == KeyRelease))
	{
		if ((event->xkey.state & Mod1Mask) ||
		    (event->xkey.state & Mod2Mask))
		{
			return(True);
		}
	}
	return(False);
}

/* check for quit */
int
IsMetaQ(event, string)
XEvent *event;
char *string;
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
