// demo for chapter 3

// headers
#include "myxlib.h"

// draw the shit
void
redraw(Display *display, Window window, GC gc)
{
	XDrawLine(display, window, gc, 1, 1, 100, 100);
	XDrawRectangle(display, window, gc, 100, 100, 100, 100);
	XFillRectangle(display, window, gc, 200, 200, 100, 100);
	DrawOval(display, window, gc, 100, 100, 100, 100);
	FillOval(display, window, gc, 110, 110, 64, 64);
	XFlush(display);
	return;
}

// main routine
main(int argc, char **argv)
{
	// connect to x-server
	int screen;
	Window rootwindow;
	Display *display = ConnectToServer(NULL, screen, rootwindow);
	if (display == NULL)
	{
		ERROR("connect to server failed.", EINVAL);
		return(2);
	}

	// create a window
	int x = 10;
	int y = 10;
	int width = 300;
	int height = 300;
	Visual *visual = CopyFromParent;
	Window window = OpenWindow(display, 
			rootwindow,
			x, y, width, height,
			BlackPixel(display, screen),
			WhitePixel(display, screen),
			ExposureMask, visual);

	// set hints for window
	SetStandardHints(display, window, argv[0], argv[0],
			x, y, width, height);

	// create GC for drawing
	GC gc = CreateGC(display, window, 
			BlackPixel(display, screen),
			WhitePixel(display, screen));

	// map window
	XMapRaised(display, window);
	XFlush(display);

	// event loop: max of 20 events, then exit.
	XEvent event;
	for (int count = 0; count < 20; )
	{
		XNextEvent(display, &event);
		if (event.type == Expose)
		{
			redraw(display, window, gc);
			count++;
		}
	}

	// close connection
	XCloseDisplay(display);

	// all done
	return(0);
}
