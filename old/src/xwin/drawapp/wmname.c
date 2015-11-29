/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* set window name */
void
SetWindowName(display, window, name)
Display *display;
Window window;
char *name;
{
	/* set window name */
	XStoreName(display, window, name);

	/* all done */
	return;
}
