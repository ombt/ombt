/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* set up color map for a screen */
Pixmap
LoadBitmap(display, window, filename, width, height)
Display *display;
Window window;
char *filename;
int *width, *height;
{
	int x_hotspot, y_hotspot;
	int status;
	Pixmap pixmap;

	/* read a bitmap file */
	status = XReadBitmapFile(display, window, filename, 
			width, height, &pixmap, &x_hotspot, &y_hotspot);
	if (status != BitmapSuccess)
	{
		pixmap = (Pixmap)None;
	}

	/* all done */
	return(pixmap);
}

/* save an bitmap in a file */
int
SaveBitmap(display, filename, bitmap, width, height)
Display *display;
char *filename;
Pixmap bitmap;
int width, height;
{
	return(XWriteBitmapFile(display, filename, 
			bitmap, width, height, -1, -1));
}
