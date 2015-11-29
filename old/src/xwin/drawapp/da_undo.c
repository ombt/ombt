/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"
#include "drawapp.h"

/* global pixmap */
Pixmap undo_pixmap;
GC undo_gc;

/* create undo pixmap */
int
CreateUndoPixmap(display, parent, width, height)
Display *display;
Window parent;
int width, height;
{
	/* create pixmap */
	undo_pixmap = XCreatePixmap(display, parent, width, height, 1);
	if (undo_pixmap == (Pixmap)None) return(False);

	/* create graphics */
	undo_gc = CreateGC(display, undo_pixmap, 0L, 0L);

	/* clear pixmap */
	XFillRectangle(display, undo_pixmap, undo_gc, 0, 0, width, height);

	/* set gc for draw */
	XSetForeground(display, undo_gc, 1L);
	return(True);
}

/* free pixmap data */
void
DestroyUndoPixmap(display)
Display *display;
{
	XFreePixmap(display, undo_pixmap);
	XFreeGC(display, undo_gc);
	return;
}

/* undo operations */
void
MaintainUndo(display)
Display *display;
{
	extern int editing_width, editing_height;
	extern Pixmap drawing_pixmap;

	/* save current image */
	CopyPlane(display, drawing_pixmap, undo_pixmap, undo_gc,
		0, 0, editing_width, editing_height, editing_width,
		editing_height);
	return;
}

void
Undo(id)
int id;
{
	extern int drawing_color;
	extern int editing_width, editing_height;
	extern Pixmap drawing_pixmap;
	extern GC drawing_gc, pixmap_gc;
	extern Window drawing_window;
	int old_color;
	Display *display;

	/* undo last operation */
	display = ToolGetDisplay(id);
	old_color = drawing_color;
	SetDrawAppColor(display, DRAWING_BLACK);
	CopyPlane(display, undo_pixmap, drawing_pixmap, pixmap_gc,
		0, 0, editing_width, editing_height, editing_width,
		editing_height);
	CopyPlane(display, undo_pixmap, drawing_window, drawing_gc,
		0, 0, editing_width, editing_height, editing_width,
		editing_height);
	SetDrawAppColor(display, old_color);
	UnhighlightPushButton(id);
	XFlush(display);
	return;
}
