// headers
#include "myxlib.h"

// draw a string
void
DrawString(Display *display, Window window, GC gc, int x, int y, char *string)
{
	// write string
	XDrawImageString(display, window, gc, x, y, string, strlen(string));

	// all done
	return;
}
