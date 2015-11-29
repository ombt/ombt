/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* local definitions */
#define PUSH_EV_MASK (ButtonPress | ExposureMask)

/* colors externs */
extern unsigned long dimgrey, lightgrey, black, white;

/* ARGSUSED */
void
PushButtonEvent(display, id, event, keysym, string)
Display *display;
int id;
XEvent *event;
KeySym keysym;
char *string;
{
	/* remove highlight */
	ToolHighlightGroup(id, ToolGetGroup(id), white, dimgrey);

	/* highlight this item */
	ToolSetState(id, True);
	ToolHighlight(id, dimgrey, white);
	XFlush(display);

	/* user callback */
	ToolExec(id);
	return;
}

/* ARGSUSED */
void
PushButtonDraw(display, id, event)
Display *display;
int id;
XExposeEvent *event;
{
	/* check count */
	if (event->count != 0) return;

	/* draw border */
	if (ToolGetState(id) == True)
		ToolHighlight(id, dimgrey, white);
	else
		ToolHighlight(id, white, dimgrey);
	
	/* draw text */
	ToolSetForeground(id, black);
	ToolDrawName(id);
	return;
}

void
UnhighlightPushButton(id)
int id;
{
	ToolSetState(id, False);
	ToolHighlight(id, white, dimgrey);
	return;
}

/* create a push button */
int
CreatePushButton(display, parent, x, y, width, height, gc, font_struct, 
		name, callback)
Display *display;
Window parent;
int x, y, width, height;
GC gc;
XFontStruct *font_struct;
char *name;
void (*callback)();
{
	Visual *visual = CopyFromParent;
	int id;

	/* create a window */
	id = CreateToolWindow(display, parent, x, y, width, height,
			lightgrey, lightgrey, PUSH_EV_MASK, visual);
	if (id >= 0)
	{
		ToolSetName(id, name);
		ToolSetGC(id, gc);
		ToolSetForeground(id, black);
		ToolSetBackground(id, lightgrey);
		ToolSetFont(id, font_struct);
		ToolSetFunctions(id, PushButtonDraw, 
				PushButtonEvent, callback);
	}
	else
	{
		ERRORI("bad id returned from CreateToolWindow.", id, EINVAL);
	}

	/* all done */
	return(id);
}
