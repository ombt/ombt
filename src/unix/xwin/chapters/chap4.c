// demo for chapter 2

// headers
#include "myxlib.h"

// global colors
unsigned long black, white, red;
unsigned long green, blue, magenta;

// draw the shit
void
redraw(Display *display, Window window, GC gc)
{
	XSetForeground(display, gc, blue);
	XDrawLine(display, window, gc, 1, 1, 100, 100);

	XSetForeground(display, gc, red);
	XDrawRectangle(display, window, gc, 100, 100, 100, 100);

	XSetForeground(display, gc, green);
	XFillRectangle(display, window, gc, 200, 200, 100, 100);

	XSetForeground(display, gc, black);
	DrawOval(display, window, gc, 100, 100, 100, 100);

	XSetForeground(display, gc, magenta);
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

	// find a pseudo-color visual for the window
	int depth;
	Visual *visual = CopyFromParent;
	int status = SetUpVisual(display, screen, visual, depth);
	if (status != True)
	{
		ERROR("unable to find pseudo-color visual.", EINVAL);
		XCloseDisplay(display);
		exit(2);
	}

	// create a window
	int x = 10;
	int y = 10;
	int width = 300;
	int height = 300;
	Window window = OpenWindow(display, 
			rootwindow,
			x, y, width, height,
			BlackPixel(display, screen),
			WhitePixel(display, screen),
			ExposureMask, visual);

	// set hints for window
	SetStandardHints(display, window, argv[0], argv[0],
			x, y, width, height);

	// set up colormap
	Colormap colormap;
	status = SetUpColormap(display, screen, window, 
			visual, colormap);
	if (status != True)
	{
		ERROR("unable to find colormap.", EINVAL);
		XCloseDisplay(display);
		exit(2);
	}

	// set up colors in colormap
	black = AllocNamedColor(display, colormap, "black",
			BlackPixel(display, screen));
	white = AllocNamedColor(display, colormap, "white",
			BlackPixel(display, screen));
	red = AllocNamedColor(display, colormap, "red",
			BlackPixel(display, screen));
	blue = AllocNamedColor(display, colormap, "blue",
			BlackPixel(display, screen));
	green = AllocNamedColor(display, colormap, "green",
			BlackPixel(display, screen));
	magenta = AllocNamedColor(display, colormap, "magenta",
			BlackPixel(display, screen));

	// create GC for drawing
	GC gc = CreateGC(display, window, black, white);

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
