/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"
#include "drawapp.h"

/* global pixmap */
Pixmap drawing_pixmap;
GC pixmap_gc;

/* externs */
extern XFontStruct *drawapp_font;

/* create drawing pixmap */
int
CreateDrawingPixmap(display, parent, width, height)
Display *display;
Window parent;
int width, height;
{
	/* create pixmap */
	drawing_pixmap = XCreatePixmap(display, parent, width, height, 1);

	/* create graphics */
	pixmap_gc = CreateGC(display, drawing_pixmap, 0L, 0L);

	/* clear pixmap */
	XFillRectangle(display, drawing_pixmap, pixmap_gc, 0, 0, width, height);

	/* set gc for draw */
	XSetForeground(display, pixmap_gc, 1L);

	/* set font */
	XSetFont(display, pixmap_gc, drawapp_font->fid);
	if ((drawing_pixmap != (Pixmap)None) && (pixmap_gc != (GC)None))
		return(True);
	else
		return(False);
}

/* free pixmap data */
void
DestroyDrawPixmap(display)
Display *display;
{
	XFreePixmap(display, drawing_pixmap);
	XFreeGC(display, pixmap_gc);
	return;
}
