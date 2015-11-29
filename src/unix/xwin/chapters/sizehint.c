// headers 
#include "myxlib.h"

// fill in size hints structure
void
FillSizeHints(int x, int y, int width, int height, XSizeHints &sizehints)
{
	// set values
	sizehints.x = x;
	sizehints.y = y;
	sizehints.height = height;
	sizehints.width = width;
	sizehints.min_height = height;
	sizehints.min_width = width;
	sizehints.flags = USPosition | USSize | PMinSize;
	sizehints.base_width = width;
	sizehints.base_height = height;
	sizehints.flags |= PBaseSize;

	// all done 
	return;
}

// set window size and location hints
void
SetSizeHints(Display *display, Window window, 
		int x, int y, int width, int height)
{
	XSizeHints sizehints;

	// get hint sizes 
	FillSizeHints(x, y, width, height, sizehints);

	// set size hints 
	XSetWMNormalHints(display, window, &sizehints);

	// all done
	return;
}

