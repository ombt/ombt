/* headers */
#include <stdio.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"
#include "drawapp.h"

/* globals */
int drawing_last_x = 0;
int drawing_last_y = 0;
int drawing_x = 0;
int drawing_y = 0;
char drawing_string[400];
Window drawing_window;
GC update_gc;
GC drawing_gc;
GC xor_gc;
int editing_width = 0;
int editing_height = 0;

/* externs */
extern int drawing_mode;
extern int drawing_color;
extern int drawing_shape;
extern Pixmap drawing_pixmap;
extern GC pixmap_gc;
extern XFontStruct *drawapp_font;
extern unsigned long black, white;

/* create editing graphics */
void
CreateEditingGCs(display, window)
Display *display;
Window window;
{
	xor_gc = CreateXorGC(display, window, black, white);
	drawing_gc = CreateGC(display, window, black, white);
	update_gc = CreateGC(display, window, black, white);
	XSetFont(display, drawing_gc, drawapp_font->fid);
	XSetFont(display, xor_gc, drawapp_font->fid);
	return;
}

/* create window */
void
CreateEditingWindow(display, parent, argc, argv, x, y, width, height,
			eventmask, visual)
Display *display;
Window parent;
int argc;
char **argv;
int x, y, width, height;
unsigned long eventmask;
Visual *visual;
{
	int id, status, i;
	char *parameter;

	/* search command line for window size */
	parameter = FindParameter(argc, argv, "-size");
	if (parameter != (char *)NULL)
	{
		i = sscanf(parameter, "%dx%d", &editing_width, &editing_height);
		if (i < 1) editing_width = width;
		if (editing_width < 16) editing_width = 16;
		if (i < 2) editing_height = height;
		if (editing_height < 16) editing_height = 16;
	}
	else
	{
		editing_width = width;
		editing_height = height;
	}

	/* create window */
	id = CreateToolWindow(display, parent, x, y, editing_width, 
			editing_height, black, white, eventmask, visual);
	ToolSetGroup(id, -99);

	/* set handler functions */
	ToolSetFunctions(id, EditingDrawFunc, EditingEventFunc, NullUserFunc);

	/* set drawing window global */
	drawing_window = ToolGetWindow(id);

	/* create graphics */
	CreateEditingGCs(display, drawing_window);

	/* create pixmaps for drawing */
	status = CreateDrawingPixmap(display, parent, editing_width,
					editing_height);
	if (status == True)
	{
		status = CreateUndoPixmap(display, parent, editing_width,
					editing_height);
	}
	if (status == False)
	{
		ERROR("failure creating bit maps.", EINVAL);
		XCloseDisplay(display);
		exit(2);
	}
	*drawing_string = 0;
	return;
}

/* resize a window */
void
ResizeEditingWindow(width, height)
int width, height;
{
	if ((editing_width != width) || (editing_height != height))
	{
		ToolResize(EDITING_WINDOW, width, height);
		editing_width = width;
		editing_height = height;
	}
	return;
}

/* ARGSUSED */
void
EditingDrawFunc(display, id, event)
Display *display;
int id;
XExposeEvent *event;
{
	UpdateWindowFromBitmap(display, drawing_window, update_gc, 
		drawing_pixmap, event, editing_width, editing_height);
	return;
}

/* ARGSUSED */
void
EditingUserFunc(id)
int id;
{
	return;
}

/* ARGSUSED */
void
EditingEventFunc(display, id, event, keysym, string)
Display *display;
int id;
XEvent *event;
KeySym keysym;
char *string;
{
	if (drawing_mode == DRAWING)
	{
		DrawShape(display, drawing_window, xor_gc, drawing_shape, 
			drawing_last_x, drawing_last_y, drawing_x, drawing_y);
	}
	switch (event->type)
	{
	case MotionNotify:
		if (drawing_mode == DRAWING)
		{
			drawing_x = event->xbutton.x;
			drawing_y = event->xbutton.y;
		}
		break;
	case ButtonPress:
		if (drawing_mode != DRAWING)
		{
			drawing_mode = DRAWING;
			drawing_last_x = event->xbutton.x;
			drawing_last_y = event->xbutton.y;
			drawing_x = event->xbutton.x;
			drawing_y = event->xbutton.y;
			*drawing_string = 0;
		}
		break;
	case ButtonRelease:
		if (drawing_mode == DRAWING)
		{
			drawing_mode = NOT_DRAWING;
			drawing_x = event->xbutton.x;
			drawing_y = event->xbutton.y;
			DrawItem(display, drawing_shape, drawing_last_x, 
				drawing_last_y, drawing_x, drawing_y);
		}
		break;
	case KeyPress:
		if (drawing_shape == DRAW_TEXT)
		{
			drawing_mode = DRAWING;
			EditText(display, id, keysym, string);
		}
		break;
	}
	if (drawing_mode == DRAWING)
	{
		DrawShape(display, drawing_window, xor_gc, drawing_shape, 
			drawing_last_x, drawing_last_y, drawing_x, drawing_y);
	}
	return;
}

/* set drawing color */
void
SetDrawingColor(id)
int id;
{
	Display *display;
	display = ToolGetDisplay(id);
	SetDrawAppColor(display, id);
	return;
}

/* remove something */
void
WipeOut(id)
int id;
{
	int old_color;
	Display *display;
	display = ToolGetDisplay(id);
	old_color = drawing_color;
	SetDrawAppColor(display, DRAWING_WHITE);
	DrawItem(display, FILL_RECT, 0, 0, editing_width, editing_height);
	SetDrawAppColor(display, old_color);
	UnhighlightPushButton(id);
	XFlush(display);
	return;
}

void
EditText(display, id, keysym, string)
Display *display;
int id;
KeySym keysym;
char *string;
{
	int length;
	if (keysym < 255)
	{
		if (isprint(keysym))
		{
			strcat(drawing_string, string);
		}
	}
	else
	{
		switch (keysym)
		{
		case XK_Return:
			DrawItem(display, drawing_shape, drawing_last_x,
				drawing_last_y, drawing_x, drawing_y);
			drawing_mode = NOT_DRAWING;
			*drawing_string = 0;
			break;

		case XK_Delete:
		case XK_BackSpace:
			length = strlen(drawing_string);
			if (length > 0)
			{
				ToolRedraw(id);
				length--;
				drawing_string[length] = 0;
				if (length > 0)
				{
					DrawShape(display, drawing_window, 
						xor_gc, drawing_shape, 
						drawing_last_x, drawing_last_y, 
						drawing_x, drawing_y);
				}
			}
			break;
		}
	}
	return;
}
