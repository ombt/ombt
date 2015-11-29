/* headers */
#include <stdio.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"
#include "drawapp.h"

/* globals */
char drawing_string[400];
Window drawing_window;
Pixmap drawing_pixmap;
GC pixmap_gc;
GC drawing_gc;

/* externs */
extern int drawing_mode;
extern int drawing_color;
extern unsigned long black, white;

/* draw an item */
void
DrawItem(display, shape, x1, y1, x2, y2)
Display *display;
int shape;
int x1, y1, x2, y2;
{
	extern int changed_flag;

	MaintainUndo(display);
	DrawShape(display, drawing_window, drawing_gc, shape,
		x1, y1, x2, y2);
	DrawShape(display, drawing_pixmap, pixmap_gc, shape,
		x1, y1, x2, y2);
	changed_flag = BITMAP_CHANGED;
	return;
}

/* draw a shape */
void
DrawShape(display, drawable, gc, shape, x1, y1, x2, y2)
Display *display;
Drawable drawable;
GC gc;
int shape;
int x1, y1, x2, y2;
{
	int width, height;

	height = y2 - y1;
	width = x2 - x1;

	switch (shape)
	{
	case DRAW_LINE:
		XDrawLine(display, drawable, gc, x1, y1, x2, y2);
		break;
	case DRAW_POINTS:
		XDrawPoint(display, drawable, gc, x2, y2);
		break;
	case DRAW_RECT:
		if ((height > 1) && (width > 1))
		{
			XDrawRectangle(display, drawable, gc, 
					x1, y1, width, height);
		}
		break;
	case FILL_RECT:
		if ((height > 1) && (width > 1))
		{
			XFillRectangle(display, drawable, gc, 
					x1, y1, width, height);
		}
		break;
	case DRAW_OVAL:
		if ((height > 1) && (width > 1))
		{
			DrawOval(display, drawable, gc, x1, y1, width, height);
		}
		break;
	case FILL_OVAL:
		if ((height > 1) && (width > 1))
		{
			FillOval(display, drawable, gc, x1, y1, width, height);
		}
		break;
	case DRAW_TEXT:
		if (strlen(drawing_string) > 0)
		{
			XDrawImageString(display, drawable, gc, x1, y1, 
				drawing_string, strlen(drawing_string));
		}
		break;
	}
	return;
}

/* set foreground and background colors */
void
SetDrawAppColor(display, color)
Display *display;
int color;
{
	drawing_mode = NOT_DRAWING;
	drawing_color = color;
	if (drawing_color == DRAWING_BLACK)
	{
		XSetForeground(display, drawing_gc, black);
		XSetBackground(display, drawing_gc, white);
		XSetForeground(display, pixmap_gc, 1L);
		XSetBackground(display, pixmap_gc, 0L);
	}
	else
	{
		XSetForeground(display, drawing_gc, white);
		XSetBackground(display, drawing_gc, black);
		XSetForeground(display, pixmap_gc, 0L);
		XSetBackground(display, pixmap_gc, 1L);
	}
	return;
}

