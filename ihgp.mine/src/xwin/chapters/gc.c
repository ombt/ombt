/* headers */
#include "myxlib.h"

// create a graphics context
GC
CreateGC(Display *display, Drawable drawable, unsigned long forecolor, unsigned long backcolor)
{
	XGCValues xgcvalues;
	GC gc;

	// set colors
	xgcvalues.foreground = forecolor;
	xgcvalues.background = backcolor;

	// set values
	gc = XCreateGC(display, drawable, (GCForeground | GCBackground),
			&xgcvalues);

	// all done
	return(gc);
}
