/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* create a transient window */
int
CreateTransientWindow(display, parent, owner, x, y, width, height, 
		border, backcolor, event_mask, visual, colormap,
		app_name, wind_name)
Display *display;
Window parent, owner;
int x, y, width, height;
unsigned long border, backcolor;
unsigned long event_mask;
Visual *visual;
Colormap colormap;
char *app_name, *wind_name;
{
	Window window;

	/* create a window */
	window = OpenWindow(display, parent, x, y, width, height,
			border, backcolor, event_mask, visual);
	if (window != (Window)None)
	{
		XSetTransientForHint(display, window, owner);
		SetStandardHints(display, window, app_name, wind_name,
			x, y, width, height);
		XSetWindowColormap(display, window, colormap);
		XSetWindowBackground(display, window, backcolor);
	}

	/* all done */
	return(window);
}
