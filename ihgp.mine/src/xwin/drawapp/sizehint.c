/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* fill in size hints structure */
void
FillSizeHints(x, y, width, height, sizehints)
int x, y, width, height;
XSizeHints *sizehints;
{
	/* set values */
	sizehints->x = x;
	sizehints->y = y;
	sizehints->height = height;
	sizehints->width = width;
	sizehints->min_height = height;
	sizehints->min_width = width;
	sizehints->flags = USPosition | USSize | PMinSize;
#ifdef X11R4
	sizehints->base_width = width;
	sizehints->base_height = height;
	sizehints->flags |= PBaseSize;
#endif

	/* all done */
	return;
}

/* set window size and location hints */
void
SetSizeHints(display, window, x, y, width, height)
Display *display;
Window window;
int x, y, width, height;
{
	XSizeHints sizehints;

	/* get hint sizes */
	FillSizeHints(x, y, width, height, &sizehints);

	/* set size hints */
#ifdef X11R4
	XSetWMNormalHints(display, window, &sizehints);
#else
	XSetNormalHints(display, window, &sizehints);
#endif

	/* all done */
	return;
}

