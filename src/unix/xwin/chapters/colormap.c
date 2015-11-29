// headers 
#include "myxlib.h"

// set up color map for a screen
int
SetUpColormap(Display *display, int screen, 
	Window window, Visual *visual, Colormap &colormap)
{
	int status = False;

	// look for a pseudo-color visual
	if (visual == DefaultVisual(display, screen))
	{
		colormap = DefaultColormap(display, screen);
		status = True;
	}
	else
	{
		if (visual == CopyFromParent) 
			visual = DefaultVisual(display, screen);
		colormap = XCreateColormap(display, window, visual, AllocNone);
		if (colormap != None)
		{
			XSetWindowColormap(display, window, colormap);
			status = True;
		}
		else
		{
			colormap = DefaultColormap(display, screen);
		}
	}

	// all done 
	return(status);
}
