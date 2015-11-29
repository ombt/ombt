/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* local definitions */
#define APPL_CLASS "Examples"

/* open a window */
Window
TopWindow(display, parent, argc, argv, x, y, width, height, 
	bordercolor, backcolor, event_mask, visual)
Display *display;
Window parent;
int argc;
char **argv;
int *x, *y;
int *width, *height;
unsigned long bordercolor;
unsigned long backcolor;
unsigned long event_mask;
Visual *visual;
{
	Window window;
	int screen;
	int initial_state;

	/* check window geometry */
	screen = DefaultScreen(display);
	CheckGeometry(argc, argv, DisplayWidth(display, screen),
		DisplayHeight(display, screen), x, y, width, height);

	/* create window */
	window = OpenWindow(display, parent, *x, *y, *width, *height,
			bordercolor, backcolor, event_mask, visual);

	/* send info to window manager */
	SetSizeHints(display, window, *x, *y, *width, *height);
	SetClassHints(display, window, argv[0], APPL_CLASS);

	/* set window name */
	FindWindowName(display, window, argc, argv);

	/* should window be an icon */
	initial_state = FindInitialState(argc, argv);
	SetWMHints(display, window, initial_state);

	/* set WM_COMMAND property */
	XSetCommand(display, window, argv, argc);

	/* all done */
	return(window);
}

/* should window be an icon */
int
FindInitialState(argc, argv)
int argc;
char **argv;
{
	char *ptr;
	ptr = FindParameter(argc, argv, "-icon");
	if (ptr != (char *)NULL)
		return(IconicState);
	else
		return(NormalState);
}

/* was a window name given */
void
FindWindowName(display, window, argc, argv)
Display *display;
Window window;
int argc;
char **argv;
{
	char *ptr, window_name[300];

	/* default name */
	strcpy(window_name, argv[0]);
	
	/* get name from command line, if any */
	ptr = FindParameter(argc, argv, "-name");
	if (ptr != (char *)NULL)
	{
		strcpy(window_name, ptr);
	}
	else
	{
		ptr = FindParameter(argc, argv, "-title");
		if (ptr != (char *)NULL)
		{
			strcpy(window_name, ptr);
		}
	}
	SetWindowName(display, window, window_name);

	/* all done */
	return;
}
