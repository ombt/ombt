/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* local definitions */
#define BORDER_WIDTH 2
#define APPL_CLASS "Examples"

/* open a window */
Window
OpenWindow(display, parent, x, y, width, height, bordercolor, 
	backcolor, event_mask, visual)
Display *display;
Window parent;
int x,y;
int width, height;
unsigned long bordercolor;
unsigned long backcolor;
unsigned long event_mask;
Visual *visual;
{
	Window window;
	XSetWindowAttributes attributes;
	unsigned long attr_mask;

	/* set up window attributes */
	attributes.event_mask = event_mask;
	attributes.border_pixel = bordercolor;
	attributes.background_pixel = backcolor;
	attr_mask = CWEventMask | CWBackPixel | CWBorderPixel;

#if 0
	/* tell window-manager to leave window alone */
	attributes.override_redirect = True;
	attr_mask |= CWOverrideRedirect;
#endif

	/* create window */
	window = XCreateWindow(display, parent, x, y, width, height,
			BORDER_WIDTH, CopyFromParent, InputOutput,
			visual, attr_mask, &attributes);

	/* all done */
	return(window);
}

/* set hints */
void
SetStandardHints(display, window, app_name, wind_name, x, y, 
	width, height)
Display *display;
Window window;
char *app_name;
char *wind_name;
int x, y, width, height;
{
	/* set hints */
	SetSizeHints(display, window, x, y, width, height);
	SetWindowName(display, window, app_name, APPL_CLASS);
	SetWMHints(display, window, NormalState);

	/* all done */
	return;
}
