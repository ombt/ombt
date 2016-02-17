// demo for chapter 2

// headers
#include "myxlib.h"

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
			count++;
			cout << "Expose event (x,y) =  (";
			cout << event.xexpose.x << "," << event.xexpose.y;
			cout << ")" << endl;
			
			cout << "Expose event (w,h) =  (";
			cout << event.xexpose.width << "," << event.xexpose.height;
			cout << ")" << endl;
		}
	}

	// close connection
	XCloseDisplay(display);

	// all done
	return(0);
}
