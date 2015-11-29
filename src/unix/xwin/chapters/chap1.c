// demo for chapter 1

// headers
#include "myxlib.h"

// get x info and print it
void
PrintXInfo(Display *display, int screen)
{
	// vendor, etc. info
	cout << "server vendor is ... " << ServerVendor(display) << endl;
	cout << "version is ... " << VendorRelease(display) << endl;

	// protocol version
	cout << "X protocol version ... " << ProtocolVersion(display) << endl;
	cout << "X protocol revision ... " << ProtocolRevision(display) << endl;

	// screen depth
	int depth = DefaultDepth(display, screen);
	if (depth == 1)
		cout << "monochrome screen, depth is ... " << depth << endl;
	else
		cout << "color plane screen, depth is ... " << depth << endl;

	// get width and height of screen
	cout << "screen width is ... " << DisplayWidth(display,screen) << endl;
	cout << "screen height is ... " << DisplayHeight(display,screen) << endl;

	// black and white pixel values
	cout << "black pixel is ... " << BlackPixel(display, screen) << endl;
	cout << "white pixel is ... " << WhitePixel(display, screen) << endl;

	// display name
	cout << "display name ... " << XDisplayName(NULL) << endl;

	// all done
	return;
}

// main routine
main(int , char **)
{
	int screen;
	Window rootwindow;

	// connect to x-server
	Display *display = ConnectToServer(NULL, screen, rootwindow);
	if (display == NULL)
	{
		ERROR("connect to server failed.", EINVAL);
		return(2);
	}

	// get x-server info and print it
	PrintXInfo(display, screen);

	// close connection
	XCloseDisplay(display);

	// all done
	return(0);
}
