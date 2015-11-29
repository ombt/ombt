/* headers */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* maximum name length */
#define MAX_NAME_LEN 80

/* maximum tool kit windows */
#define MAX_TOOL_WINDOWS 40

/* check range of id */
#define ID_OK(WindowId) (((WindowId) >= 0) && ((WindowId) < tool_max_id))

/* access tool window array */
#define TWA(WindowId) TWArray[(WindowId)]

/* structure for holding tool kit info */
typedef struct {
	Display *display;
	Window parent;
	Window window;
	GC gc;
	XFontStruct *font;
	int width, height;
	int state;
	int group;
	int id;
	char name[MAX_NAME_LEN+1];
	void (*drawfunc)();
	void (*userfunc)();
	void (*eventfunc)();
} ToolWindow;

/* global tool kit windows */
static ToolWindow TWArray[MAX_TOOL_WINDOWS+1];
static int tool_max_id = 0;

/* create a tool window */
int
CreateToolWindow(display, parent, x, y, width, height, 
		bordercolor, backcolor, event_mask, visual)
Display *display;
Window parent;
int x, y, width, height;
unsigned long bordercolor;
unsigned long backcolor;
unsigned long event_mask;
Visual *visual;
{
	int id;

	/* check if there is a free slot */
	if (tool_max_id >= MAX_TOOL_WINDOWS)
	{
		ERRORI("too many ids is use.", tool_max_id, EINVAL);
		return(-1);
	}

	/* open a window */
	id = tool_max_id;
	TWA(id).window = OpenWindow(display, parent, x, y, width, height, 
				bordercolor, backcolor, event_mask, visual);

	/* was a window allocated */
	if (TWA(id).window == (Window)None)
	{
		ERRORI("invalid window was returned.", TWA(id).window, EINVAL);
		return(-1);
	}

	/* fill in window structure */
	TWA(id).display = display;
	TWA(id).parent = parent;
	TWA(id).width = width;
	TWA(id).height = height;
	TWA(id).state = False;
	TWA(id).group = 0;
	TWA(id).drawfunc = NullDrawFunc;
	TWA(id).userfunc = NullUserFunc;
	TWA(id).userfunc = NullEventFunc;
	TWA(id).name[0] = 0;

	/* next window */
	tool_max_id++;

	/* return id */
	return(id);
}

/* translate id to tool display */
Display *
ToolGetDisplay(id)
int id;
{
	if (ID_OK(id))
	{
		return(TWA(id).display);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
		return(TWA(0).display);
	}
}

/* translate id to tool window */
Window
ToolGetWindow(id)
int id;
{
	if (ID_OK(id))
	{
		return(TWA(id).window);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
		return(TWA(0).window);
	}
}

/* set background color for a window */
void
ToolSetBackground(id, color)
int id;
unsigned long color;
{
	if (ID_OK(id))
	{
		XSetBackground(TWA(id).display, TWA(id).gc, color);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* set foreground color for a window */
void
ToolSetForeground(id, color)
int id;
unsigned long color;
{
	if (ID_OK(id))
	{
		XSetForeground(TWA(id).display, TWA(id).gc, color);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* set graphic contents */
void
ToolSetGC(id, gc)
int id;
GC gc;
{
	if (ID_OK(id))
	{
		TWA(id).gc = gc;
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* set font for a window */
void
ToolSetFont(id, font_struct)
int id;
XFontStruct *font_struct;
{
	if (ID_OK(id))
	{
		TWA(id).font = font_struct;
		XSetFont(TWA(id).display, TWA(id).gc, 
			TWA(id).font->fid);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* translate id to group */
int
ToolGetGroup(id)
int id;
{
	if (ID_OK(id))
	{
		return(TWA(id).group);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
		return(-1);
	}
}

/* set group */
void
ToolSetGroup(id, group)
int id;
int group;
{
	if (ID_OK(id))
	{
		TWA(id).group = group;
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* translate id to state */
int
ToolGetState(id)
int id;
{
	if (ID_OK(id))
	{
		return(TWA(id).state);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
		return(False);
	}
}

/* set state */
void
ToolSetState(id, state)
int id;
int state;
{
	if (ID_OK(id))
	{
		TWA(id).state = state;
	}
	return;
}

/* translate id to name */
void
ToolGetName(id, name)
int id;
char *name;
{
	if (ID_OK(id))
		strcpy(name, TWA(id).name);
	else
		*name = 0;
	return;
}

/* set state */
void
ToolSetName(id, name)
int id;
char *name;
{
	if (ID_OK(id) && (strlen(name) < MAX_NAME_LEN))
	{
		strcpy(TWA(id).name, name);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* center and write some text */
void
ToolCenterText(id, text)
int id;
char *text;
{
	int x, y, width, height;

	if (ID_OK(id))
	{
		height = FontHeight(TWA(id).font);
		width = XTextWidth(TWA(id).font, text, strlen(text));
		x = (TWA(id).width - width)/2;
		y = (TWA(id).height - height)/2 + TWA(id).font->ascent;
		XDrawImageString(TWA(id).display, 
				TWA(id).window, TWA(id).gc, 
				x, y, text, strlen(text));
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* write centered name */
void
ToolDrawName(id)
int id;
{
	char name[MAX_NAME_LEN+1];
	ToolGetName(id, name);
	ToolCenterText(id, name);
	return;
}

/* set functions for window */
void
ToolSetFunctions(id, drawfunc, eventfunc, userfunc)
int id;
void (*drawfunc)();
void (*eventfunc)();
void (*userfunc)();
{
	if (ID_OK(id))
	{
		TWA(id).drawfunc = drawfunc;
		TWA(id).eventfunc = eventfunc;
		TWA(id).userfunc = userfunc;
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* set user function for window */
void
ToolSetUserFunc(id, userfunc)
int id;
void (*userfunc)();
{
	if (ID_OK(id))
	{
		TWA(id).userfunc = userfunc;
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* execute a window callback */
void
ToolExec(id)
int id;
{
	if (ID_OK(id))
	{
		(*TWA(id).userfunc)(id);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* default functions */
/* ARGSUSED */
void
NullDrawFunc(display, id, event, keysym, string)
Display *display;
int id;
XExposeEvent *event;
KeySym keysym;
char *string;
{
}

void
NullUserFunc(id)
int id;
{
}

void
NullEventFunc(display, id, event)
Display *display;
int id;
XEvent *event;
{
}

/* highlight a group of windows */
void
ToolHighlightGroup(static_id, group, top, bottom)
int static_id;
int group;
unsigned long top, bottom;
{
	int id;
	for (id = 0; id < tool_max_id; id++)
	{
		if ((ToolGetGroup(id) == group) && (id != static_id))
		{
			ToolHighlight(id, top, bottom);
		}
	}
	return;
}

/* high light a window */
void
ToolHighlight(id, top, bottom)
int id;
unsigned long top, bottom;
{
	/* check id */
	if ( ! ID_OK(id)) return;

	/* draw top part of window */
	ToolSetForeground(id, top);
	ToolDrawLine(id, 0, 0, TWA(id).width, 0);
	ToolDrawLine(id, 0, 1, TWA(id).width-1, 1);
	ToolDrawLine(id, 0, 0, 0, TWA(id).height);
	ToolDrawLine(id, 1, 1, 1, TWA(id).height-1);

	/* draw bottom part of window */
	ToolSetForeground(id, bottom);
	ToolDrawLine(id, 0, TWA(id).height, TWA(id).width, 
		TWA(id).height);
	ToolDrawLine(id, 1, TWA(id).height-1, TWA(id).width-1, 
		TWA(id).height-1);
	ToolDrawLine(id, TWA(id).width, TWA(id).height, 
		TWA(id).width, 0);
	ToolDrawLine(id, TWA(id).width-1, TWA(id).height-1, 
		TWA(id).width-1, 1);
	return;
}

/* draw a line */
void
ToolDrawLine(id, x1, y1, x2, y2)
int id;
int x1, y1, x2, y2;
{
	if (ID_OK(id))
	{
		XDrawLine(TWA(id).display, TWA(id).window,
			TWA(id).gc, x1, y1, x2, y2);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* clear a window */
void
ToolClear(id)
int id;
{
	if (ID_OK(id))
	{
		XClearWindow(TWA(id).display, TWA(id).window);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* dispatch an event for a window */
void
ToolRedraw(id)
int id;
{
	XEvent event;
	if (ID_OK(id))
	{
		event.xexpose.x = 0;
		event.xexpose.y = 0;
		event.xexpose.width = TWA(id).width;
		event.xexpose.height = TWA(id).height;
		event.xexpose.count = 0;
		DispatchEvent(id, &event);
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* resize a window */
void
ToolResize(id, new_width, new_height)
int id;
int new_width, new_height;
{
	if (ID_OK(id))
	{
		if ((TWA(id).width != new_width) ||
		    (TWA(id).height != new_height))
		{
			TWA(id).width = new_width;
			TWA(id).height = new_height;
			XResizeWindow(TWA(id).display, TWA(id).window, 
				TWA(id).width, TWA(id).height);
		}
	}
	else
	{
		ERRORI("bad id value.", id, EINVAL);
	}
	return;
}

/* send event to a window */
int
ToolEvent(event)
XEvent *event;
{
	int id, i;

	/* get window for event */
	id = -1;
	for (i = 0; i < tool_max_id; i++)
	{
		if (TWA(i).window == event->xany.window)
		{
			id = i;
			break;
		}
	}

	/* send event to window */
	if (id >= 0)
	{
		DispatchEvent(id, event);
		return(True);
	}
	ERRORI("no id found for window.", event->xany.window, EINVAL);
	return(False);
}

/* call event handler for a window */
int
DispatchEvent(id, event)
int id;
XEvent *event;
{
	int done = False;
	char string[100];
	KeySym keysym;

	/* initialize strings */
	*string = 0;
	keysym = 0;

	/* handle event */
	switch(event->type)
	{
	case Expose:
		(*TWA(id).drawfunc)(TWA(id).display, id, event);
		break;
	case KeyPress:
		done = DecodeKeyPress(event, &keysym, string);
	case ButtonPress:
	case ButtonRelease:
	case MotionNotify:
		(*TWA(id).eventfunc)(TWA(id).display, id, 
				event, keysym, string);
		break;
	case MappingNotify:
		XRefreshKeyboardMapping(event);
		break;
	default:
		(*TWA(id).eventfunc)(TWA(id).display, id, 
				event, keysym, string);
		break;
	}
	return(done);
}
