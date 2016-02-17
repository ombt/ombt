/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* local definitions */
#define LABEL_EV_MASK ExposureMask

/* colors externs */
extern unsigned long black, lightgrey;

/* write a label */
/* ARGSUSED */
void
LabelDraw(display, id, event)
Display *display;
int id;
XExposeEvent *event;
{
	if (event->count == 0)
	{
		ToolSetForeground(id, black);
		ToolDrawName(id);
	}
	return;
}

/* write a label */
int
CreateTextLabel(display, parent, x, y, width, height, gc, font_struct, label)
Display *display;
Window parent;
int x, y, width, height;
GC gc;
XFontStruct *font_struct;
char *label;
{
	Visual *visual = CopyFromParent;
	int id;

	/* create a window */
	id = CreateToolWindow(display, parent, x, y, width, height,
			lightgrey, lightgrey, LABEL_EV_MASK, visual);
	if (id >= 0)
	{
		ToolSetName(id, label);
		ToolSetGC(id, gc);
		ToolSetForeground(id, black);
		ToolSetBackground(id, lightgrey);
		ToolSetFont(id, font_struct);
		ToolSetGroup(id, -999);
		ToolSetFunctions(id, LabelDraw, NullEventFunc, NullUserFunc);
	}

	/* all done */
	return(id);
}
