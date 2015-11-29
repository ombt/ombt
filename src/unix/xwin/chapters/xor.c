// headers
#include "myxlib.h"

// create a GC for xor-drawing
GC
CreateXorGC(Display *display, Drawable drawable, 
	unsigned long forecolor, unsigned long backcolor)
{
	XGCValues xgcvalues;
	GC gc;

	// set colors
	xgcvalues.foreground = forecolor ^ backcolor;
	xgcvalues.background = 0;
	xgcvalues.function = GXxor;

	// set values
	gc = XCreateGC(display, drawable, 
			(GCFunction | GCForeground | GCBackground),
			&xgcvalues);

	// all done
	return(gc);
}
