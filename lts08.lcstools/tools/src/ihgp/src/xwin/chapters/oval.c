// headers
#include "myxlib.h"

// local definitions
#undef START_CIRCLE
#define START_CIRCLE 0

#undef FULL_CIRCLE
#define FULL_CIRCLE (360*64)

// draw an oval
void
DrawOval(Display *display, Drawable drawable, GC gc, 
	int x, int y, int width, int height)
{
	// check parameters 
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	// draw oval
	XDrawArc(display, drawable, gc, x, y, width, height,
		START_CIRCLE, FULL_CIRCLE);

	// all done
	return;
}

// fill an oval
void
FillOval( Display *display, Drawable drawable, GC gc, 
	int x, int y, int width, int height)
{
	// check parameters
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	// draw oval
	XFillArc(display, drawable, gc, x, y, width, height,
		START_CIRCLE, FULL_CIRCLE);

	// all done
	return;
}

// remove these
#undef START_CIRCLE
#undef FULL_CIRCLE

