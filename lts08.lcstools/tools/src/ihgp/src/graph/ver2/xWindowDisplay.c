/* functions for X-Window display class */

/* headers */
#include "debug.h"
#include "returns.h"
#include "xWindowDisplay.h"

/* constructors and destructor */
XWindowDisplay::XWindowDisplay(char *dname, int debug)
{
	init(dname, debug);
}

XWindowDisplay::~XWindowDisplay()
{
	deleteAll();
}

/* initialization functions */
int
XWindowDisplay::init(char *dname, int debug)
{
	/* save display name, if any */
	if (dname == (char *)0)
	{
		displayName = (char *)0;
	}
	else
	{
		displayName = new char [strlen(dname)+1];
		if (displayName == (char *)0)
		{
			status = ENOMEM;
			ERROR("unable to allocate display name.", status);
			return(NOTOK);
		}
		strcpy(displayName, dname);
	}

	/* connect to X-server */
	display = XOpenDisplay(displayName);
	if (display == (Display *)0)
	{
		status = EINVAL;
		ERRORS("unable to open display.", XDisplayName(displayName), 
			status);
		return(NOTOK);
	}

	/* synchronize for debugging */
	if (debug)
		XSynchronize(display, True);
	else
		XSynchronize(display, False);

	/* get default screen */
	screen = DefaultScreen(display);

	/* get root window id */
	rootWindow = RootWindow(display, screen);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindowDisplay::deleteAll()
{
	/* close connection to X-server */
	XCloseDisplay(display);

	/* delete display name */
	if (displayName != (char *)0)
	{
		delete [] displayName;
		displayName = (char *)0;
	}

	/* all done */
	status = OK;
	return(OK);
}

/* turn X-window synchronization on or off */
int
XWindowDisplay::xSyncOn()
{
	/* turn on synch for debugging */
	XSynchronize(display, True);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindowDisplay::xSyncOff()
{
	/* turn on synch for debugging */
	XSynchronize(display, False);

	/* all done */
	status = OK;
	return(OK);
}
