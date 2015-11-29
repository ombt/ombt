// demo for chapter 2

// headers
#include "myxlib.h"

// font names
#define FONT_NAME "variable"
#define FALLBACK_FONT_NAME "fixed"

// global colors
unsigned long black, white, red;
unsigned long green, blue, magenta;

// draw the shit
void
redraw(Display *display, Window window, GC gc, XFontStruct *font_struct)
{
	XSetForeground(display, gc, blue);
	XFillRectangle(display, window, gc, 100, 10, 100, 100);

	int y = 30;

	char string[BUFSIZ];
	strcpy(string, "XDrawImageString() test");

	XSetForeground(display, gc, red);
	XDrawImageString(display, window, gc, 30, y, string, strlen(string));

	y += FontHeight(font_struct) + 5;

	strcpy(string, "XDrawString() test");
	
	XSetForeground(display, gc, green);
	XDrawString(display, window, gc, 30, y, string, strlen(string));

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

	// load a font
	XFontStruct *font_struct = 
		LoadFont(display, FONT_NAME, FALLBACK_FONT_NAME);

	// set up gc with font structure
	XSetFont(display, gc, font_struct->fid);

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
			redraw(display, window, gc, font_struct);
			count++;
		}
	}

	// close connection
	XFreeFont(display, font_struct);
	XCloseDisplay(display);

	// all done
	return(0);
}
