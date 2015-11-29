/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* connect to an X server */
Display *
ConnectToServer(display_name, screen, rootwindow)
char *display_name;
int *screen;
Window *rootwindow;
{
	Display *display;

	/* connect to X server */
	display = XOpenDisplay(display_name);
	if (display == (Display *)NULL)
	{
		ERRORS("cannot connect to X server.\n",
			XDisplayName(display_name), EINVAL);
		exit(2);
	}

	/* get screen number */
	*screen = DefaultScreen(display);

	/* get root window id */
	*rootwindow = RootWindow(display, *screen);

	/* all done */
	return(display);
}
