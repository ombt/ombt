// headers
#include "myxlib.h"

// set up color map for a screen
Pixmap
LoadBitmap(Display *display, Window window, char *filename, 
	int *width, int *height)
{
	int x_hotspot, y_hotspot;
	int status;
	Pixmap pixmap;

	// read a bitmap file
	status = XReadBitmapFile(display, window, filename, 
			(unsigned int *)width, (unsigned int *)height, 
			&pixmap, &x_hotspot, &y_hotspot);
	if (status != BitmapSuccess)
	{
		pixmap = (Pixmap)None;
	}

	// all done
	return(pixmap);
}

// save an bitmap in a file
int
SaveBitmap(Display *display, char *filename, Pixmap bitmap,
	 int width, int height)
{
	return(XWriteBitmapFile(display, filename, 
			bitmap, width, height, -1, -1));
}
