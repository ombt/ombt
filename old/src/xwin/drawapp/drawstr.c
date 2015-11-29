/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* draw a string */
void
DrawString(display, window, gc, x, y, string)
Display *display;
Window window;
GC gc;
int x, y;
char *string;
{
	/* write string */
	XDrawImageString(display, window, gc, x, y, string, strlen(string));

	/* all done */
	return;
}
