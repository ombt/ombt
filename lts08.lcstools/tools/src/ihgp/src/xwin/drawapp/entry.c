/* headers */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* local definitions */
#define ENTRY_EV_MASK (KeyPressMask | ExposureMask)

/* colors externs */
extern unsigned long dimgrey, lightgrey, black, white;

/* ARGSUSED */
void
TextEntryEvent(display, id, event, keysym, string)
Display *display;
int id;
XEvent *event;
KeySym keysym;
char *string;
{
	int length;
	char name[200];

	ToolGetName(id, name);
	if (keysym < 255)
	{
		if (isprint(keysym))
		{
			strcat(name, string);
			ToolSetName(id, name);
			ToolSetForeground(id, black);
			ToolDrawName(id);
		}
	}
	else
	{
		switch (keysym)
		{
		case XK_Return:
			ToolExec(id);
			break;
		case XK_Delete:
		case XK_BackSpace:
			length = strlen(name);
			if (length > 0)
			{
				length--;
				name[length] = '\0';
				ToolSetName(id, name);
				ToolClear(id);
				ToolHighlight(id, dimgrey, white);
				ToolSetForeground(id, black);
				ToolDrawName(id);
			}
			break;
		}
	}
	return;
}

/* ARGSUSED */
void
TextEntryDraw(display, id, event)
Display *display;
int id;
XExposeEvent *event;
{
	if (event->count != 0) return;
	ToolHighlight(id, dimgrey, white);
	ToolSetForeground(id, black);
	ToolDrawName(id);
	return;
}

int
CreateTextEntry(display, parent, x, y, width, height, gc, font_struct,
		name, callback)
Display *display;
Window parent;
int x, y, width, height;
XFontStruct *font_struct;
char *name;
void (*callback)();
{
	Visual *visual = CopyFromParent;
	int id;

	id = CreateToolWindow(display, parent, x, y, width, height,
			lightgrey, lightgrey, ENTRY_EV_MASK, visual);
	if (id >= 0)
	{
		ToolSetName(id, name);
		ToolSetGC(id, gc);
		ToolSetForeground(id, black);
		ToolSetBackground(id, lightgrey);
		ToolSetFont(id, font_struct);
		ToolSetGroup(id, -97);
		ToolSetFunctions(id, TextEntryDraw, TextEntryEvent, callback);
	}
	return(id);
}
