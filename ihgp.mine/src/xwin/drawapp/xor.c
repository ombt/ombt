/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* create a GC for xor-drawing */
GC
CreateXorGC(display, drawable, forecolor, backcolor)
Display *display;
Drawable drawable;
unsigned long forecolor;
unsigned long backcolor;
{
	XGCValues xgcvalues;
	GC gc;

	/* set colors */
	xgcvalues.foreground = forecolor ^ backcolor;
	xgcvalues.background = 0;
	xgcvalues.function = GXxor;

	/* set values */
	gc = XCreateGC(display, drawable, 
			(GCFunction | GCForeground | GCBackground),
			&xgcvalues);

	/* all done */
	return(gc);
}
