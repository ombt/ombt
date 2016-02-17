/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* update window from a pixmap */
void
UpdateWindowFromBitmap(display, window, gc, bitmap, event, 
			max_width, max_height)
Display *display;
Window window;
GC gc;
Pixmap bitmap;
XExposeEvent *event;
int max_width, max_height;
{
	/* update window */
	CopyPlane(display, bitmap, window, gc, 
		event->x, event->y, event->width, event->height,
		max_width, max_height);

	/* all done */
	return;
}

void
CopyPlane(display, src_draw, dest_draw, gc, x, y, width, height, 
		max_width, max_height)
Display *display;
Drawable src_draw, dest_draw;
GC gc;
int x, y, width, height;
int max_width, max_height;
{
	/* update a valid area */
	if (((x+width) <= max_width) && ((height+y) <= max_height) &&
	     (x < max_width) && (y < max_height))
	{
		XCopyPlane(display, src_draw, dest_draw, gc,
			x, y, width, height, x, y, 0x1);
	}

	/* all done */
	return;
}

