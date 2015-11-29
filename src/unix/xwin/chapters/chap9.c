// demo for chapter 9

// headers
#include "myxlib.h"

// font names
#define FONT_NAME "variable"
#define FALLBACK_FONT_NAME "fixed"

// global colors
unsigned long black, white;

// events to receive
#define EVENT_MASK (ButtonPressMask|KeyPressMask|ExposureMask)

// icon file name
#define ICON_FILENAME "iocn.xbm"

// main routine
main(int argc, char **argv)
{
	char string[BUFSIZ];

	// give help message
	cout << "buttom clicks in window will draw dots." << endl;
	cout << "alt-q to quit." << endl;

	// connect to x-server
	int screen;
	Window rootwindow;
	Display *display = ConnectToServer(NULL, screen, rootwindow);
	if (display == NULL)
	{
		ERROR("connect to server failed.", EINVAL);
		return(2);
	}

	// get basic colors
	black = BlackPixel(display, screen);
	white = WhitePixel(display, screen);

	// create a window
	Visual *visual = CopyFromParent;
	int x = 10;
	int y = 10;
	int width = 300;
	int height = 300;
	Window window = OpenWindow(display, 
			rootwindow,
			x, y, width, height,
			black, white,
			EVENT_MASK, visual);

	// set hints for window
	SetStandardHints(display, window, argv[0], argv[0],
			x, y, width, height);

	// load up icon file
	LoadIcon(display, window, ICON_FILENAME, argv[0]);
	
	// create GC for drawing
	GC gc = CreateGC(display, window, black, white);

	// create a bit map
	Pixmap bitmap = XCreatePixmap(display, rootwindow, 
				width, height, 1);

	if (bitmap == (Pixmap)None)
	{
		ERROR("unable to create pixmap", errno);
		XCloseDisplay(display);
		exit(2);
	}

	// create gc for pixmap
	GC pix_gc = CreateGC(display, bitmap, 0L, 0L);

	// clear bitmap
	XFillRectangle(display, bitmap, pix_gc, 0, 0, width, height);
	XSetForeground(display, pix_gc, 1L);

	// map window
	XMapRaised(display, window);
	XFlush(display);

	// event loop
	KeySym keysym;
	XEvent event;
	for (int done = False; done != True; )
	{
		XNextEvent(display, &event);
		switch (event.type)
		{
		case Expose:
			UpdateWindowFromBitmap(display, window, 
				gc, bitmap, &event.xexpose, width, height);
			XFlush(display);
			break;

		case ButtonPress:
			XDrawPoint(display, window, gc, 
				event.xbutton.x, event.xbutton.y);
			XDrawPoint(display, bitmap, pix_gc, 
				event.xbutton.x, event.xbutton.y);
			XFlush(display);
			break;

		case KeyPress:
			done = DecodeKeyPress(&event.xkey, &keysym, string);
			break;

		case MappingNotify:
			XRefreshKeyboardMapping(&event.xmapping);
			break;
		}
	}

	// close connection
	XCloseDisplay(display);

	// all done
	return(0);
}
