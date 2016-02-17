/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* create a graphics context */
GC
CreateGC(display, drawable, forecolor, backcolor)
Display *display;
Drawable drawable;
unsigned long forecolor;
unsigned long backcolor;
{
	XGCValues xgcvalues;
	GC gc;

	/* set colors */
	xgcvalues.foreground = forecolor;
	xgcvalues.background = backcolor;

	/* set values */
	gc = XCreateGC(display, drawable, (GCForeground | GCBackground),
			&xgcvalues);

	/* all done */
	return(gc);
}
