// demo for chapter 9

// headers
#include "myxlib.h"

// global colors
unsigned long black, white;

// globals for drawing
int start_x = -1;
int start_y = -1;
int last_x = -1;
int last_y = -1;

// events to receive
#define EVENT_MASK \
	(ButtonPressMask| \
	ButtonReleaseMask| \
	ButtonMotionMask| \
	KeyPressMask| \
	ExposureMask)

// icon file name
#define ICON_FILENAME "icon.xbm"


// draw an item
void 
DrawItem(Display *display, Drawable drawable, GC gc, 
	int x1, int y1, int x2, int y2)
{
	int width = x2 - x1;
	if (width < 0) width = 0;
	int height = y2 - y1;
	if (height < 0) height = 0;
#if 0
	DrawOval(display, drawable, gc, x1, y1, width, height);
#else
	XDrawRectangle(display, drawable, gc, x1, y1, width, height);
#endif
	return;
}

// main routine
main(int argc, char **argv)
{
	char string[BUFSIZ];

	// give help message
	cout << "holding button and dragging will draw a shape." << endl;
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
	GC xor_gc = CreateXorGC(display, window, black, white);

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
			start_x = event.xbutton.x;
			start_y = event.xbutton.y;
			last_x = start_x;
			last_y = start_y;

			DrawItem(display, window, xor_gc, 
				start_x, start_y, last_x, last_y);
			XFlush(display);
			break;

		case ButtonRelease:
			DrawItem(display, window, xor_gc, 
				start_x, start_y, last_x, last_y);
			DrawItem(display, window, gc, 
				start_x, start_y, last_x, last_y);
			DrawItem(display, bitmap, pix_gc, 
				start_x, start_y, last_x, last_y);
			XFlush(display);
			start_x = -1;
			start_y = -1;
			last_x = -1;
			last_y = -1;
			break;

		case MotionNotify:
			DrawItem(display, window, xor_gc, 
				start_x, start_y, last_x, last_y);
			last_x = event.xmotion.x;
			last_y = event.xmotion.y;
			DrawItem(display, window, xor_gc, 
				start_x, start_y, last_x, last_y);
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
