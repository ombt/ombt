/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* setup connection to X server */
Display *
StartupX(argc, argv, screen, rootwindow, black, white, message)
int argc;
char **argv;
int *screen;
Window *rootwindow;
unsigned long *black, *white;
char *message;
{
	Display *display;

	/* check for help request */
	if (CheckForHelp(argc, argv, message) == True) exit(0);

	/* open display to X server */
	display = OpenDisplay(argc, argv, screen, rootwindow);

	/* set up error handlers */
	SetErrorHandlers();

	/* set up colors */
	*black = BlackPixel(display, *screen);
	*white = WhitePixel(display, *screen);

	/* all done */
	return(display);
}
