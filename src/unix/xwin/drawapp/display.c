/* headers */
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* find display name in argv */
int
FindDisplayName(argc, argv, display_name)
int argc;
char **argv;
char *display_name;
{
	char *ptr;

	/* get display name */
	ptr = FindParameter(argc, argv, "-disp");
	if (ptr != (char *)NULL)
	{
		strcpy(display_name, ptr);
		return(True);
	}
	else
	{
		*display_name = 0;
		return(False);
	}
}

/* open a connection to an X server */
Display *
OpenDisplay(argc, argv, screen, rootwindow)
int argc;
char **argv;
int *screen;
Window *rootwindow;
{
	char display_name[BUFSIZ];
	Display *display;

	/* get name from command line, if any */
	FindDisplayName(argc, argv, display_name);

	/* open connection */
	display = ConnectToServer(display_name, screen, rootwindow);

	/* all done */
	return(display);
}
