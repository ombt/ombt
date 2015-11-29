// headers
#include "myxlib.h"

// update window from a pixmap
void
UpdateWindowFromBitmap(Display *display, Window window, GC gc, Pixmap bitmap,
		XExposeEvent *event, int max_width, int max_height)
{
	// update window
	CopyPlane(display, bitmap, window, gc, 
		event->x, event->y, event->width, event->height,
		max_width, max_height);
	return;
}

void
CopyPlane(Display *display, Drawable src_draw, Drawable dest_draw, GC gc,
	int x, int y, int width, int height, int max_width, int max_height)
{
	// update a valid area
	if (((x+width) <= max_width) && ((height+y) <= max_height) &&
	     (x < max_width) && (y < max_height))
	{
		XCopyPlane(display, src_draw, dest_draw, gc,
			x, y, width, height, x, y, 0x1);
	}
	return;
}

