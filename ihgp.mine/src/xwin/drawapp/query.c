/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* get mouse location */
void
QueryPointer(display, x, y)
Display *display;
int *x, *y;
{
	int status, screen;
	Window rootwindow, childwindow;
	int rootx, rooty, childx, childy;
	unsigned int button_state;

	/* get location of mouse */
	screen = DefaultScreen(display);
	status = XQueryPointer(display, RootWindow(display, screen),
			&rootwindow, &childwindow, &rootx, &rooty,
			&childx, &childy, &button_state);
	if (status == True)
	{
		*x = rootx;
		*y = rooty;
	}
	else
	{
		*x = 0;
		*y = 0;
	}

	/* all done */
	return;
}
