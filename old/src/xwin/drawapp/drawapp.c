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
#include "drawapp.h"

/* font names */
#define FONT_NAME "variable"
#define FALLBACK_FONT_NAME "fixed"

/* event mask */
#define EV_MASK (ButtonPressMask | \
		 ButtonReleaseMask | \
		 ButtonMotionMask | \
		 KeyPressMask | \
		 ExposureMask)

/* other definitions */
#define MAX_WIDTH 48
#define MAX_4_WIDTH 192
#define BUTTON_WIDTH 60

/* icon file name */
#define ICON_FILENAME "drawapp.xbm"

/* globals */
unsigned long black, white, lightgrey, dimgrey;
int drawing_shape = DRAW_LINE;
int drawing_mode = NOT_DRAWING;
int drawing_color = DRAWING_BLACK;
int changed_flag = BITMAP_SAVED;
XFontStruct *drawapp_font;
Window main_window;

/* set drawing shape for window */
void
SetDrawingShape(id)
int id;
{
	drawing_mode = NOT_DRAWING;
	drawing_shape = id;
	return;
}

/* create button windows */
void
CreateButtons(display, window, gc, font_struct)
Display *display;
Window window;
GC gc;
XFontStruct *font_struct;
{
	int y, height, id;

	/* get font height */
	height = FontHeight(font_struct)+4;
	y = 6;

	/* create buttons */
	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Line", SetDrawingShape);
	ToolSetGroup(id, 1);
	ToolSetState(id, True);
	id = CreatePushButton(display, window, 68, y, BUTTON_WIDTH,
		height, gc, font_struct, "Points", SetDrawingShape);
	ToolSetGroup(id, 1);
	y += height + 2;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Rect", SetDrawingShape);
	ToolSetGroup(id, 1);
	id = CreatePushButton(display, window, 68, y, BUTTON_WIDTH,
		height, gc, font_struct, "Fill Rect", SetDrawingShape);
	ToolSetGroup(id, 1);
	y += height + 2;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Oval", SetDrawingShape);
	ToolSetGroup(id, 1);
	id = CreatePushButton(display, window, 68, y, BUTTON_WIDTH,
		height, gc, font_struct, "Fill Oval", SetDrawingShape);
	ToolSetGroup(id, 1);
	y += height + 2;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Text", SetDrawingShape);
	ToolSetGroup(id, 1);
	y += height + 12;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Black", SetDrawingColor);
	ToolSetGroup(id, 2);
	ToolSetState(id, True);
	id = CreatePushButton(display, window, 68, y, BUTTON_WIDTH,
		height, gc, font_struct, "White", SetDrawingColor);
	ToolSetGroup(id, 2);
	y += height + 12;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "  Wipe Out", WipeOut);
	id = CreatePushButton(display, window, 68, y, BUTTON_WIDTH,
		height, gc, font_struct, "Undo", Undo);
	y += height + 12;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Save", SaveFile);
	id = CreatePushButton(display, window, 68, y, BUTTON_WIDTH,
		height, gc, font_struct, "Load...", PopUpLoadFile);
	y += height + 2;

	id = CreatePushButton(display, window, 6, y, BUTTON_WIDTH,
		height, gc, font_struct, "Quit", Quit);
	return;
}

/* handle events */
void
EventLoop(display)
Display *display;
{
	XEvent event;
	int done = False;
	int handled;
	KeySym keysym;
	char string[100];

	/* event loop */
	while ( ! done)
	{
		XNextEvent(display, &event);
		handled = ToolEvent(&event);
		if (event.type == KeyPress)
		{
			done = DecodeKeyPress(&event, &keysym, string);
			if (IsMetaKey(&event) == True) 
			{
				HandleMetaKey(string);
			}
		}
	}
	return;
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	Window rootwindow;
	int screen;
	int x, y, width, height;
	Visual *visual = CopyFromParent;
	GC gc;
	char string[200];
	int depth, status;
	Colormap colormap;
	Window dialog;

#ifdef DEBUG
	/* turn on debug flag */
	extern int _Xdebug;
	_Xdebug = 1;
#endif
	/* connect to server */
	display = StartupX(argc, argv, &screen, &rootwindow, 
			&black, &white, "");

	/* load a font */
	strcpy(string, FONT_NAME);
	drawapp_font = LoadFontName(display, argc, argv, string,
			FALLBACK_FONT_NAME);

	/* find pseudo-color visual, if any */
#ifdef NOCOLOR
	visual = DefaultVisual(display, screen);
	depth = DefaultDepth(display, screen);
#else
	status = SetUpVisual(display, screen, &visual, &depth);
	if (status == False)
	{
		ERROR("no PseudoColor Visual was found, using default.", status);
	}
#endif

	/* set up window sizes */
	x = 10;
	y = 10;
	width = (BUTTON_WIDTH*2)+MAX_4_WIDTH+24;
	height = (FontHeight(drawapp_font)+1)*14;

	/* create a top level window */
	main_window = TopWindow(display, rootwindow, argc, argv, 
			&x, &y, &width, &height, black, white,
			KeyPressMask, visual);

	/* set up color map */
#ifdef NOCOLOR
	colormap = DefaultColormap(display, screen);
#else
	status = SetUpColormap(display, screen, main_window, visual,
				&colormap);
	if (status < 1)
	{
		ERROR("SetUpColormap failed.", (int)status);
	}
#endif

	/* set up colors */
#ifdef NOCOLOR
	black = BlackPixel(display, screen);
	white = WhitePixel(display, screen);
	lightgrey = WhitePixel(display, screen);
	dimgrey = BlackPixel(display, screen);
#else
	black = AllocNamedColor(display, colormap, "black", 
				BlackPixel(display, screen));
	white = AllocNamedColor(display, colormap, "white", 
				WhitePixel(display, screen));
	lightgrey = AllocNamedColor(display, colormap, "LightGrey", 
				WhitePixel(display, screen));
	dimgrey = AllocNamedColor(display, colormap, "DimGrey", 
				BlackPixel(display, screen));
#endif

	/* load icon name */
	LoadIcon(display, main_window, ICON_FILENAME, argv[0]);

	/* create graphics context */
	gc = CreateGC(display, main_window, black, lightgrey);

	/* draw with our font */
	XSetFont(display, gc, drawapp_font->fid);

	/* create drawing window */
	CreateEditingWindow(display, main_window, argc, argv, 136, 6,
			MAX_4_WIDTH, MAX_4_WIDTH, EV_MASK, visual);

	/* create drawing window */
	CreateButtons(display, main_window, gc, drawapp_font);

	/* create dialog box */
	dialog = CreateDialog(display, rootwindow, main_window, visual,
			colormap, drawapp_font, argv[0], 
			"enter name of file to load", NullUserFunc);

	/* set default file name */
	SetFileName(display, main_window, "test.xbm");

	/* check for different file name */
	CheckForFileLoad(display, main_window, argc, argv);

	/* make window appear */
	XMapRaised(display, main_window);
	XMapSubwindows(display, main_window);
	XFlush(display);

	/* indicate that bitmap is not changed */
	changed_flag = BITMAP_SAVED;

	/* loop on events */
	EventLoop(display);

	/* save output file */
	Quit(QUIT_ID);

	/* all done */
	exit(0);
}
