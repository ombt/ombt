/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* set up color map for a screen */
unsigned long
AllocNamedColor(Display *display, Colormap colormap, char *colorname, unsigned long default_color)
{
	XColor hardwarecolor, exactcolor;
	unsigned long color;
	int status;

	/* look for given color */
	status = XAllocNamedColor(display, colormap, colorname,
			&hardwarecolor, &exactcolor);
	if (status != 0)
	{
		color = hardwarecolor.pixel;
	}
	else
	{
		color = default_color;
	}

	/* all done */
	return(color);
}
