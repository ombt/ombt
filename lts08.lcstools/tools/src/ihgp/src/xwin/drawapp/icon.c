/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* load a bitmap as a window icon */
void
LoadIcon(display, window, filename, iconname)
Display *display;
Window window;
char *filename, *iconname;
{
	Pixmap icon;
	int icon_width, icon_height;

	/* read bitmap from a file */
	icon = LoadBitmap(display, window, filename, 
			&icon_width, &icon_height);
	if (icon != (Pixmap)None)
	{
		/* set icon for window */
		SetWindowIcon(display, window, icon, iconname);
	}

	/* all done */
	return;
}

/* set window icon */
int
SetWindowIcon(display, window, icon, iconname)
Display *display;
Window window;
Pixmap icon;
char *iconname;
{
	XWMHints *wmhints;

	/* get current wm hints */
	wmhints = XGetWMHints(display, window);
	if (wmhints == (XWMHints *)0)
	{
		wmhints = (XWMHints *)malloc(sizeof(XWMHints));
		if (wmhints == (XWMHints *)0) return(False);
		wmhints->flags = 0;
	}

	/* set up icon */
	wmhints-> flags |= IconPixmapHint;
	wmhints->icon_pixmap = icon;
	XSetWMHints(display, window, wmhints);
	XFree(wmhints);

	/* set up icon name, use older R3 function */
	XSetIconName(display, window, iconname);

	/* all done */
	return;
}
