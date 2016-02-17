// headers
#include "myxlib.h"

// connect to an X server 
Display *
ConnectToServer(char *display_name, int &screen, Window &rootwindow)
{
	Display *display;

	// connect to X server
	display = XOpenDisplay(display_name);
	if (display == NULL)
	{
		ERRORD("cannot connect to X server.\n",
			XDisplayName(display_name), EINVAL);
		return(NULL);
	}

	// get screen number
	screen = DefaultScreen(display);

	// get root window id
	rootwindow = RootWindow(display, screen);

	// all done
	return(display);
}
