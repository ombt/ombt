// headers
#include "myxlib.h"

// local definitions
#define BORDER_WIDTH 2
#define APPL_CLASS "Examples"

// open a window
Window
OpenWindow(Display *display, Window parent, 
	int x, int y, int width, int height,
	unsigned long bordercolor, unsigned long backcolor,
	unsigned long event_mask, Visual *visual)
{
	Window window;
	XSetWindowAttributes attributes;
	unsigned long attr_mask;

	// set up window attributes 
	attributes.event_mask = event_mask;
	attributes.border_pixel = bordercolor;
	attributes.background_pixel = backcolor;
	attr_mask = CWEventMask | CWBackPixel | CWBorderPixel;

#if 0
	// tell window-manager to leave window alone
	attributes.override_redirect = True;
	attr_mask |= CWOverrideRedirect;
#endif

	// create window
	window = XCreateWindow(display, parent, x, y, width, height,
			BORDER_WIDTH, CopyFromParent, InputOutput,
			visual, attr_mask, &attributes);

	// all done 
	return(window);
}

// set hints 
void
SetStandardHints(Display *display, Window window, char *app_name,
		char *wind_name, int x, int y, int width, int height)
{
	// set hints
	SetSizeHints(display, window, x, y, width, height);
	SetWindowName(display, window, wind_name);
	SetClassHints(display, window, app_name, APPL_CLASS);
	SetWMHints(display, window, NormalState);

	// all done
	return;
}
