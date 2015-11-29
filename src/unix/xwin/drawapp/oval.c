/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* local definitions */
#define START_CIRCLE 0
#define FULL_CIRCLE (360*64)

/* draw an oval */
void
DrawOval(display, drawable, gc, x, y, width, height)
Display *display;
Drawable drawable;
GC gc;
int x, y, width, height;
{
	/* check parameters */
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	/* draw oval */
	XDrawArc(display, drawable, gc, x, y, width, height,
		START_CIRCLE, FULL_CIRCLE);

	/* all done */
	return;
}

/* fill an oval */
void
FillOval(display, drawable, gc, x, y, width, height)
Display *display;
Drawable drawable;
GC gc;
int x, y, width, height;
{
	/* check parameters */
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	/* draw oval */
	XFillArc(display, drawable, gc, x, y, width, height,
		START_CIRCLE, FULL_CIRCLE);

	/* all done */
	return;
}
