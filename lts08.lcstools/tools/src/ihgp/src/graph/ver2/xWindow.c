/* functions for X-window class */

/* headers */
#include <sysent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "returns.h"
#include "debug.h"
#include "xWindow.h"
#include "ftrc.h"

/* X-window headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

/* local definitions */
#ifdef DEBUG
#define FTRC() Ftrc functionTrace(__FILE__, __LINE__)
#else
#define FTRC() 
#endif
#define POPUP_X 5
#define POPUP_Y 5
#define POPUP_BORDER_WIDTH 2
#define KEYBUFFERSIZE 64

/* error codes */
static char *ErrorCodes[] = {
	"",
	"CreateWindow", "ChangeWindowAttributes", "GetWindowAttributes",
	"DestroyWindow", "DestroySubwindows", "ChangeSaveSet",
	"ReparentWindow", "MapWindow", "MapSubwindows",
	"UnmapWindow", "UnmapSubwindows", "ConfigureWindow",
	"CirculateWindow", "GetGeometry", "QueryTree",
	"InternAtom", "GetAtomName", "ChangeProperty",
	"DeleteProperty", "GetProperty", "ListProperties",
	"SetSelectionOwner", "GetSelectionOwner", "ConvertSelection",
	"SendEvent", "GrabPointer", "UngrabPointer",
	"GrabButton", "UngrabButton", "ChangeActivePointerGrab",
	"GrabKeyboard", "UngrabKeyboard", "GrabKey",
	"UngrabKey", "AllowEvents", "GrabServer",
	"UngrabServer", "QueryPointer", "GetMotionEvents",
	"TranslateCoords", "WarpPointer", "SetInputFocus",
	"GetInputFocus", "QueryKeymap", "OpenFont",
	"CloseFont", "QueryFont", "QueryTextExtents",
	"ListFonts", "ListFontsWithInfo", "SetFontPath",
	"GetFontPath", "CreatePixmap", "FreePixmap",
	"CreateGC", "ChangeGC", "CopyGC",
	"SetDashes", "SetClipRectangles", "FreeGC",
	"ClearArea", "CopyArea", "CopyPlane",
	"PolyPoint", "PolyLine", "PolySegment",
	"PolyRectangle", "PolyArc", "FillPoly",
	"PolyFillRectangle", "PolyFillArc", "PutImage",
	"GetImage", "PolyText8", "PolyText16",
	"ImageText8", "ImageText16", "CreateColormap",
	"FreeColormap", "CopyColormapAndFree", "InstallColormap",
	"UninstallColormap", "ListInstalledColormaps", "AllocColor",
	"AllocNamedColor", "AllocColorCells", "AllocColorPlanes",
	"FreeColors", "StoreColors", "StoreNamedColor",
	"QueryColors", "LookupColor", "CreateCursor",
	"CreateGlyphCursor", "FreeCursor", "RecolorCursor",
	"QueryBestSize", "QueryExtension", "ListExtensions",
	"ChangeKeyboardMapping", "GetKeyboardMapping", "ChangeKeyboardControl",
	"GetKeyboardControl", "Bell", "ChangePointerControl",
	"GetPointerControl", "SetScreenSaver", "GetScreenSaver",
	"ChangeHosts", "ListHosts", "SetAccessControl",
	"SetCloseDownMode", "KillClient", "RotateProperties",
	"ForceScreenSaver", "SetPointerMapping", "GetPointerMapping",
	"SetModifierMapping", "GetModifierMapping", "NoOperation"
};

/* generic error handler */
static int
ErrorHandler(Display *display, XErrorEvent *event)
{
	FTRC();
	int length = 120;
	char string[130];

	/* convert to a text string and print */
	XGetErrorText(display, event->error_code, string, length);
	fprintf(stderr, "\n\nX error:\n\t%s\n",
		string);
	fprintf(stderr, "\tserial number of request: %ld\n",
		(long)event->serial);
	fprintf(stderr, "\top code: %d.%d %s\n\tError Code: %d\n",
		event->request_code, event->minor_code, 
		ErrorCodes[event->request_code], event->error_code);
	fprintf(stderr, "\tresource id of failed request: %ld\n",
		(long)event->resourceid);
	fprintf(stderr, "\ton display %s.\n",
		DisplayString(display));
	return(0);
}

/* fatal error handler */
static int
FatalErrorHandler(Display *display)
{
	FTRC();
	/* convert to a text string and print */
	fprintf(stderr, "X error: fatal IO error on display %s.\n",
		DisplayString(display));
	exit(2);
	return(-1);
}

/* constructors and destructor */
XWindow::XWindow(char *xw_name, XWindowDisplay *xw_disp, int xw_x, int xw_y, 
		int xw_xsize, int xw_ysize, char *fname): GenericWindow()
{
	FTRC();
	init(xw_name, xw_disp, xw_x, xw_y, xw_xsize, xw_ysize, fname);
}

XWindow::XWindow(char *xw_name, XWindowDisplay *xw_disp, int xw_x, int xw_y, 
		int xw_xsize, int xw_ysize, unsigned long xw_fg, 
		unsigned long xw_bg, char *fname): GenericWindow()
{
	FTRC();
	init(xw_name, xw_disp, xw_x, xw_y, xw_xsize, xw_ysize, 
		xw_fg, xw_bg, fname);
}

XWindow::XWindow(const XWindow &src): GenericWindow(src)
{
	FTRC();
	init(src);
}

XWindow::~XWindow()
{
	FTRC();
	deleteAll();
}

/* initialization functions */
int
XWindow::init(char *xw_name, XWindowDisplay *xw_disp, int xw_x, int xw_y, 
		int xw_xsize, int xw_ysize, char *fname)
{
	FTRC();
	/* verify  X-window display */
	if (xw_disp == (XWindowDisplay *)0)
	{
		status = EINVAL;
		ERROR("X-window display is null.", status);
		return(NOTOK);
	}

	/* initialize X-window display */
	return(init(xw_name, xw_disp, xw_x, xw_y, xw_xsize, xw_ysize,
		BlackPixel(xw_disp->display, xw_disp->screen),
		WhitePixel(xw_disp->display, xw_disp->screen), fname));
}

int
XWindow::init(char *xw_name, XWindowDisplay *xw_disp, int xw_x, int xw_y, 
		int xw_xsize, int xw_ysize, 
		unsigned long xw_fg, unsigned long xw_bg, char *fname)
{
	FTRC();
	/* verify and store X-window display */
	if (xw_disp == (XWindowDisplay *)0)
	{
		status = EINVAL;
		ERROR("X-window display is null.", status);
		return(NOTOK);
	}
	windowDisplay = xw_disp;

	/* store window name */
	if (xw_name != (char *)0)
	{
		windowName = new char [strlen(xw_name)+1];
		if (windowName == (char *)0)
		{
			status = ENOMEM;
			ERROR("unable to allocate window name.", status);
			return(NOTOK);
		}
		strcpy(windowName, xw_name);
	}
	else
	{
		windowName = (char *)0;
	}

	/* set window attributes */
	unsigned long mask = 
		CWCursor | CWEventMask | CWBackPixel | CWBorderPixel;
	windowCursor = XCreateFontCursor(windowDisplay->display, XC_crosshair);
	windowAttributes.cursor = windowCursor;
	windowAttributes.event_mask = eventMask = ExposureMask;
	windowAttributes.border_pixel = 
		BlackPixel(windowDisplay->display, windowDisplay->screen);
	windowAttributes.background_pixel = 
		WhitePixel(windowDisplay->display, windowDisplay->screen);
	Visual *visual = CopyFromParent;

	/* create window */
	window = XCreateWindow(windowDisplay->display, 
			windowDisplay->rootWindow,
			xw_x, xw_y, xw_xsize, xw_ysize, BORDER_WIDTH, 
			(int)CopyFromParent, InputOutput, visual, mask, 
			&windowAttributes);

	/* set size hints for window */
	sizeHints.flags = USPosition | USSize | PMinSize | PMaxSize | PBaseSize;
	sizeHints.x = xw_x;
	sizeHints.y = xw_y;
	sizeHints.width = xw_xsize;
	sizeHints.height = xw_ysize;
	sizeHints.min_width = xw_xsize;
	sizeHints.min_height = xw_ysize;
	sizeHints.base_width = xw_xsize;
	sizeHints.base_height = xw_ysize;
	XSetWMNormalHints(windowDisplay->display, window, &sizeHints);

	/* store window name */
	XStoreName(windowDisplay->display, window, windowName);

	/* set window manager hints */
	windowManagerHints.flags = InputHint | StateHint;
	windowManagerHints.initial_state = NormalState;
	windowManagerHints.input = True;
	XSetWMHints(windowDisplay->display, window, &windowManagerHints);

	/* create a graphics context block */
	graphicsContextValues.foreground = xw_fg;
	graphicsContextValues.background = xw_bg;
	graphicsContext = XCreateGC(windowDisplay->display, window,
				(GCForeground | GCBackground),
				&graphicsContextValues);

	/* load font, if given */
	if (fname == (char *)0 || *fname == (char)0)
	{
		/* no font was given */
		status = EINVAL;
		ERROR("no font name was given.", status);
		return(NOTOK);
	}
	else
	{
		/* save font name */
		fontName = new char [strlen(fname)+1];
		if (fontName == (char *)0)
		{
			status = ENOMEM;
			ERROR("ENOMEM for font name.", status);
			return(NOTOK);
		}

		/* get font info */
		font = XLoadQueryFont(windowDisplay->display, fname);
		if (font == (XFontStruct *)0)
		{
			status = EINVAL;
			ERRORS("unable to load font.", fname, status);
			return(NOTOK);
		}

		/* load font into graphics display */
		XSetFont(windowDisplay->display, graphicsContext, font->fid);
	}

	/* window NOT displayed for now */
	mapped = 0;

	/* set up error handlers */
	XSetErrorHandler(ErrorHandler);
	XSetIOErrorHandler(FatalErrorHandler);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::init(const XWindow &src)
{
	FTRC();
	/* store X-window display */
	windowDisplay = src.windowDisplay;

	/* store window name */
	if (src.windowName != (char *)0)
	{
		windowName = new char [strlen(src.windowName)+1];
		if (windowName == (char *)0)
		{
			status = ENOMEM;
			ERROR("unable to allocate window name.", status);
			return(NOTOK);
		}
		strcpy(windowName, src.windowName);
	}
	else
	{
		windowName = (char *)0;
	}

	/* set window attributes */
	unsigned long mask = 
		CWCursor | CWEventMask | CWBackPixel | CWBorderPixel;
	eventMask = src.eventMask;
	windowAttributes = src.windowAttributes;
	windowCursor = src.windowCursor;
	Visual *visual = CopyFromParent;

	/* create window */
	window = XCreateWindow(windowDisplay->display, windowDisplay->rootWindow,
			src.sizeHints.x, src.sizeHints.y, 
			src.sizeHints.width, src.sizeHints.height, BORDER_WIDTH, 
			(int)CopyFromParent, InputOutput, visual, mask, 
			&windowAttributes);

	/* set size hints for window */
	sizeHints = src.sizeHints;
	XSetWMNormalHints(windowDisplay->display, window, &sizeHints);

	/* store window name */
	XStoreName(windowDisplay->display, window, windowName);

	/* set window manager hints */
	windowManagerHints = src.windowManagerHints;
	XSetWMHints(windowDisplay->display, window, &windowManagerHints);

	/* create a graphics context block */
	graphicsContextValues = src.graphicsContextValues;
	graphicsContext = XCreateGC(windowDisplay->display, window,
				(GCForeground | GCBackground),
				&graphicsContextValues);

	/* load font, if given */
	if (src.fontName == (char *)0 || src.fontName[0] == (char)0)
	{
		/* no font was given */
		status = EINVAL;
		ERROR("no font name was given.", status);
		return(NOTOK);
	}
	else
	{
		/* save font name */
		fontName = new char [strlen(src.fontName)+1];
		if (fontName == (char *)0)
		{
			status = ENOMEM;
			ERROR("ENOMEM for font name.", status);
			return(NOTOK);
		}

		/* get font info */
		font = XLoadQueryFont(windowDisplay->display, fontName);
		if (font == (XFontStruct *)0)
		{
			status = EINVAL;
			ERRORS("unable to load font.", fontName, status);
			return(NOTOK);
		}

		/* load font into graphics display */
		XSetFont(windowDisplay->display, graphicsContext, font->fid);
	}

	/* window NOT displayed for now */
	mapped = 0;

	/* set up error handlers */
	XSetErrorHandler(ErrorHandler);
	XSetIOErrorHandler(FatalErrorHandler);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::deleteAll()
{
	FTRC();
	/* unmap the window */
	XUnmapWindow(windowDisplay->display, window);

	/* flush request buffer */
	XFlush(windowDisplay->display);

	/* delete window name */
	if (windowName != (char *)0)
	{
		delete [] windowName;
		windowName = (char *)0;
	}

	/* release font data */
	if (fontName != (char *)0)
	{
		delete [] fontName;
		fontName = (char *)0;
	}
	if (font != (XFontStruct *)0)
	{
		XFreeFont(windowDisplay->display, font);
		font = (XFontStruct *)0;
	}

	/* delete graphics context */
	XFreeGC(windowDisplay->display, graphicsContext);

	/* free cursor */
	if (windowCursor != (Cursor)None)
	{
		XFreeCursor(windowDisplay->display, windowCursor);
		windowCursor = (Cursor)None;
	}

	/* close window */
	XDestroyWindow(windowDisplay->display, window);

	/* check for any events */
	/*
	XEvent event;
	XCheckWindowEvent(windowDisplay->display, window, 
			ExposureMask, &event);
	 */

	/* null out window display */
	windowDisplay = (XWindowDisplay *)0;
	eventMask = 0;
	mapped = 0;

	/* all done */
	status = OK;
	return(OK);
}

/* assignment operator */
XWindow &
XWindow::operator=(const XWindow &rhs)
{
	FTRC();
	if (this == &rhs) return(*this);
	deleteAll();
	GenericWindow::operator=(rhs);
	init(rhs);
	return(*this);
}

/* set or get cursor location */
int 
XWindow::putCursor(int cx, int cy)
{
	FTRC();
	/* move cursor to new location */
	XWarpPointer(windowDisplay->display, None, window,
			0, 0, 0, 0, cx, cy);

	/* all done */
	status = OK;
	return(OK);
}

int 
XWindow::getCursor(int &cx, int &cy)
{
	FTRC();
	Window root_return, child_return;
	int root_x_return, root_y_return;
	int window_x_return, window_y_return;
	unsigned int mask_return;

	/* get cursor location */
	Bool bstatus = XQueryPointer(windowDisplay->display, window,
			&root_return, &child_return, 
			&root_x_return, &root_y_return,
			&window_x_return, &window_y_return,
			&mask_return);

	/* check if cursor is in this window */
	if (bstatus == True)
	{
		cx = window_x_return;
		cy = window_y_return;
		status = OK;
		return(OK);
	}
	else
	{
		cx = -1;
		cy = -1;
		status = EINVAL;
		return(NOTOK);
	}
}

/* draw points in window */
int
XWindow::putPoint(int x, int y, unsigned long color)
{
	FTRC();
	/* draw a point with the given color */
	XSetForeground(windowDisplay->display, graphicsContext, color);

	/* draw point */
	XDrawPoint(windowDisplay->display, window, graphicsContext, x, y);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::putPoint()
{
	FTRC();
	/* get cursor location */
	int cx, cy;
	if (getCursor(cx, cy) == OK)
	{
		return(putPoint(cx, cy));
	}
	else
	{
		/* error in getting cursor */
		status = EINVAL;
		return(NOTOK);
	}
}

int
XWindow::putPoint(unsigned long color)
{
	FTRC();
	/* get cursor location */
	int cx, cy;
	if (getCursor(cx, cy) == OK)
	{
		return(putPoint(cx, cy, color));
	}
	else
	{
		/* error in getting cursor */
		status = EINVAL;
		return(NOTOK);
	}
}

int
XWindow::putPoint(int x, int y)
{
	FTRC();
	/* draw point */
	XDrawPoint(windowDisplay->display, window, graphicsContext, x, y);

	/* all done */
	status = OK;
	return(OK);
}

/* draw characters in window */
int
XWindow::putCharacter(int x, int y, int symbol, unsigned long color)
{
	FTRC();
	/* store character in a string */
	char string[2];
	string[0] = (char) symbol;
	string[1] = (char) 0;

	/* write string */
	return(putString(x, y, string, color));
}

int
XWindow::putCharacter(int symbol)
{
	FTRC();
	/* store character in a string */
	char string[2];
	string[0] = (char) symbol;
	string[1] = (char) 0;

	/* write string */
	return(putString(string));
}

int
XWindow::putCharacter(int symbol, unsigned long color)
{
	FTRC();
	/* store character in a string */
	char string[2];
	string[0] = (char) symbol;
	string[1] = (char) 0;

	/* write string */
	return(putString(string, color));
}

int
XWindow::putCharacter(int x, int y, int symbol)
{
	FTRC();
	/* store character in a string */
	char string[2];
	string[0] = (char) symbol;
	string[1] = (char) 0;

	/* write string */
	return(putString(x, y, string));
}

/* write a string to the window */
int
XWindow::putString(int x, int y, char *string, unsigned long color, 
			GenericWindow::Direction dir)
{
	FTRC();
	/* check string */
	if ((string == (char *)0) || (*string == (char)0))
	{
		status = EINVAL;
		ERROR("null or null length string.", status);
		return(NOTOK);
		
	}

	/* get direction for writing string */
	int delx, dely;
	switch (dir)
	{
	case GenericWindow::Direction0:
		delx = 1;
		dely = 0;
		break;
	case GenericWindow::Direction1:
		delx = 1;
		dely = -1;
		break;
	case GenericWindow::Direction2:
		delx = 0;
		dely = -1;
		break;
	case GenericWindow::Direction3:
		delx = -1;
		dely = -1;
		break;
	case GenericWindow::Direction4:
		delx = -1;
		dely = 0;
		break;
	case GenericWindow::Direction5:
		delx = -1;
		dely = 1;
		break;
	case GenericWindow::Direction6:
		delx = 0;
		dely = 1;
		break;
	case GenericWindow::Direction7:
		delx = 1;
		dely = 1;
		break;
	default:
		status = EINVAL;
		ERROR("invalid direction.", status);
		return(NOTOK);
	}

	/* set foreground color */
	XSetForeground(windowDisplay->display, graphicsContext, color);

	/* write string on screen */
	if (dir == GenericWindow::Direction0)
	{
		/* just write string */
		XDrawString(windowDisplay->display, window, graphicsContext,
			x, y, string, strlen(string));
	}
	else
	{
		/* write string one character at a time */
		char tmpString[2];
		char *pstr = string;
		int height = font->ascent + font->descent;
		for ( ; *pstr != 0; pstr++)
		{
			/* save character */
			tmpString[0] = *pstr;
			tmpString[1] = (char)0;

			/* write string */
			XDrawString(windowDisplay->display, window, 
				graphicsContext, x, y, tmpString, 1);

			/* calculate the next location */
			x += delx*XTextWidth(font, tmpString, 1);
			y += dely*height;
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::putString(char *string, GenericWindow::Direction dir)
{
	FTRC();
	/* get cursor location */
	int cx, cy;
	if (getCursor(cx, cy) == OK)
	{
		return(putString(cx, cy, string, dir));
	}
	else
	{
		/* error in getting cursor */
		status = EINVAL;
		return(NOTOK);
	}
}

int
XWindow::putString(int x, int y, char *string, GenericWindow::Direction dir)
{
	FTRC();
	/* check string */
	if ((string == (char *)0) || (*string == (char)0))
	{
		status = EINVAL;
		ERROR("null or null length string.", status);
		return(NOTOK);
		
	}

	/* get direction for writing string */
	int delx, dely;
	switch (dir)
	{
	case GenericWindow::Direction0:
		delx = 1;
		dely = 0;
		break;
	case GenericWindow::Direction1:
		delx = 1;
		dely = -1;
		break;
	case GenericWindow::Direction2:
		delx = 0;
		dely = -1;
		break;
	case GenericWindow::Direction3:
		delx = -1;
		dely = -1;
		break;
	case GenericWindow::Direction4:
		delx = -1;
		dely = 0;
		break;
	case GenericWindow::Direction5:
		delx = -1;
		dely = 1;
		break;
	case GenericWindow::Direction6:
		delx = 0;
		dely = 1;
		break;
	case GenericWindow::Direction7:
		delx = 1;
		dely = 1;
		break;
	default:
		status = EINVAL;
		ERROR("invalid direction.", status);
		return(NOTOK);
	}

	/* write string on screen */
	if (dir == GenericWindow::Direction0)
	{
		/* just write string */
		XDrawString(windowDisplay->display, window, graphicsContext,
			x, y, string, strlen(string));
	}
	else
	{
		/* write string one character at a time */
		char tmpString[2];
		char *pstr = string;
		int height = font->ascent + font->descent;
		for ( ; *pstr != 0; pstr++)
		{
			/* save character */
			tmpString[0] = *pstr;
			tmpString[1] = (char)0;

			/* write string */
			XDrawString(windowDisplay->display, window, 
				graphicsContext, x, y, tmpString, 1);

			/* calculate the next location */
			x += delx*XTextWidth(font, tmpString, 1);
			y += dely*height;
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::putString(char *string, unsigned long color, 
			GenericWindow::Direction dir)
{
	FTRC();
	/* get cursor location */
	int cx, cy;
	if (getCursor(cx, cy) == OK)
	{
		return(putString(cx, cy, string, color, dir));
	}
	else
	{
		/* error in getting cursor */
		status = EINVAL;
		return(NOTOK);
	}
}

/* string width and length */
int
XWindow::stringHeight(char *)
{
	status = OK;
	return(font->ascent + font->descent);
}

int
XWindow::stringWidth(char *string)
{
	status = OK;
	return(XTextWidth(font, string, strlen(string)));
}

/* set font type */
int
XWindow::setFont(char *fname)
{
	/* check font name */
	if (fname == (char *)0 || *fname == 0)
	{
		status = EINVAL;
		ERROR("null font name.", status);
		return(NOTOK);
	}

	/* release font data */
	if (fontName != (char *)0)
	{
		delete [] fontName;
		fontName = (char *)0;
	}
	if (font != (XFontStruct *)0)
	{
		XFreeFont(windowDisplay->display, font);
		font = (XFontStruct *)0;
	}

	/* save font name */
	fontName = new char [strlen(fname)+1];
	if (fontName == (char *)0)
	{
		status = ENOMEM;
		ERROR("ENOMEM for font name.", status);
		return(NOTOK);
	}

	/* get font info */
	font = XLoadQueryFont(windowDisplay->display, fname);
	if (font == (XFontStruct *)0)
	{
		status = EINVAL;
		ERRORS("unable to load font.", fname, status);
		return(NOTOK);
	}

	/* load font into graphics display */
	XSetFont(windowDisplay->display, graphicsContext, font->fid);

	/* all done */
	status = OK;
	return(OK);
}

/* get a character or string */
int
XWindow::getCharacter(int &character)
{
	FTRC();
	/* get a one character string */
	char tmpString[2];
	if (getString(1, tmpString) != OK) return(NOTOK);
	character = *tmpString;

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::getString(int stringlength, char *string)
{
	FTRC();
	/* check parameters */
	if (stringlength <= 0 || string == (char *)0)
	{
		status = EINVAL;
		ERROR("null string or string length.", status);
		return(NOTOK);
	}
	*string = 0;

	/* create a pop-up window for input */
	int popup_width = 
		stringlength*font->max_bounds.width + 4;
	if ((POPUP_X + popup_width + 4) >= sizeHints.width)
		popup_width = sizeHints.width - 4;
	int popup_height = 
		font->max_bounds.ascent + font->max_bounds.descent + 4;
	if ((POPUP_Y + popup_height + 4) >= sizeHints.height)
		popup_height = sizeHints.height - 4;
	Window popup_window = 
		XCreateSimpleWindow(windowDisplay->display, window, 
		POPUP_X, POPUP_Y, popup_width, popup_height, POPUP_BORDER_WIDTH, 
		BlackPixel(windowDisplay->display, windowDisplay->screen),
		WhitePixel(windowDisplay->display, windowDisplay->screen));
	XSelectInput(windowDisplay->display, popup_window, 
			ExposureMask|KeyPressMask);
	XMapWindow(windowDisplay->display, popup_window);

	/* get start coordinates for printing */
	int start_x = 2;
	int start_y = font->max_bounds.ascent + 2;

	/* handle events for popup window */
	XEvent event;
	KeySym keySym;
	char keyBuf[KEYBUFFERSIZE+1];
	XComposeStatus composeStatus;

	/* start event loop */
	for (int done = 0; ! done; )
	{
		/* get next event for pop-up window */
		XWindowEvent(windowDisplay->display, popup_window,
			ExposureMask|KeyPressMask, &event);

		/* handle events */
		switch (event.type)
		{
		case Expose:
			/* clear window and repaint */
			XClearWindow(windowDisplay->display, popup_window);
			XDrawString(windowDisplay->display, popup_window,
				graphicsContext, start_x, start_y,
				string, strlen(string));
			XFlush(windowDisplay->display);

			/* grab control of keyboard */
			XGrabKeyboard(windowDisplay->display, popup_window, 
			False, GrabModeAsync, GrabModeAsync, CurrentTime);
			break;
		case KeyPress:
		{
			/* translate key */
			int keyLength = 
				XLookupString(&event.xkey, keyBuf, 
					KEYBUFFERSIZE, &keySym, &composeStatus);
			keyBuf[keyLength] = 0;
			if ((keySym == XK_Return) || (keySym == XK_KP_Enter) || 
			    (keySym == XK_Linefeed))
			{
				/* remove pop-up window */
				XUngrabKeyboard(windowDisplay->display, 
						CurrentTime);
				XUnmapWindow(windowDisplay->display, 
						popup_window);
				XFlush(windowDisplay->display);
				XDestroyWindow(windowDisplay->display, 
						popup_window);
				done = 1;
			}
			else if (((keySym >= XK_KP_Space) && 
				  (keySym <= XK_KP_9)) ||
			         ((keySym >= XK_space) && 
			          (keySym <= XK_asciitilde)) ||
				 ((keySym >= XK_F1) && 
				  (keySym <= XK_F35)))
			{
				if ((strlen(string) +
				     strlen(keyBuf)) >= stringlength)
				{
					XBell(windowDisplay->display, 100);
					XFlush(windowDisplay->display);
				}
				else
				{
					strcat(string, keyBuf);
					XDrawString(windowDisplay->display, 
						popup_window, graphicsContext, 
						start_x, start_y,
						string, strlen(string));
					XFlush(windowDisplay->display);
				}
			}
			else if ((keySym == XK_BackSpace) || 
				 (keySym == XK_Delete))
			{
				int tlen = strlen(string);
				if (tlen > 0)
				{
					string[tlen-1] = (char)0;
					XClearWindow(windowDisplay->display, 
						popup_window);
					XDrawString(windowDisplay->display, 
						popup_window, graphicsContext, 
						start_x, start_y,
						string, strlen(string));
					XFlush(windowDisplay->display);
				}
				else
				{
					XBell(windowDisplay->display, 100);
					XFlush(windowDisplay->display);
				}
			}
			break;
		}
		case MappingNotify:
			/* refresh keyboard mapping */
			XRefreshKeyboardMapping(&event.xmapping);
			break;
		default:
			/* ignore */
			break;
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

/* set or get foreground and background colors */
int
XWindow::setForegroundColor(unsigned long fg)
{
	FTRC();
	/* set foreground color */
	XSetForeground(windowDisplay->display, graphicsContext, fg);

	/* all done */
	status = OK;
	return(OK);
}

unsigned long
XWindow::getForegroundColor()
{
	FTRC();
	/* get graphics context data */
	XGetGCValues(windowDisplay->display, graphicsContext, 
			GCForeground, &graphicsContextValues);

	/* all done */
	status = OK;
	return(graphicsContextValues.foreground);
}

int
XWindow::setBackgroundColor(unsigned long bg)
{
	FTRC();
	/* set background color */
	XSetBackground(windowDisplay->display, graphicsContext, bg);

	/* all done */
	status = OK;
	return(OK);
}

unsigned long
XWindow::getBackgroundColor()
{
	FTRC();
	/* get graphics context data */
	XGetGCValues(windowDisplay->display, graphicsContext, 
			GCBackground, &graphicsContextValues);

	/* all done */
	status = OK;
	return(graphicsContextValues.background);
}

unsigned long
XWindow::getColorForPoint(int x, int y)
{
	FTRC();
	/* this is very inefficient */
	XImage *ximage = XGetImage(windowDisplay->display, window, 
		0, 0, sizeHints.width, sizeHints.height, AllPlanes, XYPixmap);

	/* get color for given point */
	unsigned long color = XGetPixel(ximage, x, y);

	/* all done */
	status = OK;
	return(color);
}

/* clear part or all of a window */
int
XWindow::clearWindow(int x0, int y0, int x1, int y1)
{
	FTRC();
	/* clear area in window */
	XClearArea(windowDisplay->display, window, 
		x0, y0, x1 - x0 + 1, y1 - y0 + 1, False);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::clearWindow()
{
	FTRC();
	/* clear window */
	XClearWindow(windowDisplay->display, window);

	/* all done */
	status = OK;
	return(OK);
}

/* draw a line */
int
XWindow::drawLine(int x0, int y0, int x1, int y1, unsigned long color)
{
	FTRC();
	/* draw a line with the given color */
	XSetForeground(windowDisplay->display, graphicsContext, color);

	/* draw line */
	XDrawLine(windowDisplay->display, window, graphicsContext, 
			x0, y0, x1, y1);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::drawLine(int x1, int y1)
{
	FTRC();
	/* get cursor location */
	int cx, cy;
	if (getCursor(cx, cy) == OK)
	{
		return(drawLine(cx, cy, x1, y1));
	}
	else
	{
		/* error in getting cursor */
		status = EINVAL;
		return(NOTOK);
	}
}

int
XWindow::drawLine(int x0, int y0, int x1, int y1)
{
	FTRC();
	/* draw line */
	XDrawLine(windowDisplay->display, window, graphicsContext, 
			x0, y0, x1, y1);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::drawLine(int x1, int y1, unsigned long color)
{
	FTRC();
	/* get cursor location */
	int cx, cy;
	if (getCursor(cx, cy) == OK)
	{
		return(drawLine(cx, cy, x1, y1, color));
	}
	else
	{
		/* error in getting cursor */
		status = EINVAL;
		return(NOTOK);
	}
}

/* fill a region */
int
XWindow::fillRegion(int, int, unsigned long, unsigned long)
{
	FTRC();
	/* not supported */
	status = NOTOK;
	return(NOTOK);
}

int
XWindow::fillRegion(unsigned long, unsigned long)
{
	FTRC();
	/* not supported */
	status = NOTOK;
	return(NOTOK);
}

/* draw or fill arcs */
int
XWindow::drawArc(int cx, int cy, int rx, int ry, int alpha, int arc, 
			unsigned long color)
{
	FTRC();
	/* convert to X-window arguments */
	int x = cx - rx;
	int y = cy - ry;
	unsigned int width = 2*rx;
	unsigned int height = 2*ry;
	int angle1 = 64*alpha;
	int angle2 = 64*arc;

	/* set foreground color */
	XSetForeground(windowDisplay->display, graphicsContext, color);

	/* draw line */
	XDrawArc(windowDisplay->display, window, graphicsContext, 
			x, y, width, height, angle1, angle2);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::drawArc(int cx, int cy, int rx, int ry, int alpha, int arc)
{
	FTRC();
	/* convert to X-window arguments */
	int x = cx - rx;
	int y = cy - ry;
	unsigned int width = 2*rx;
	unsigned int height = 2*ry;
	int angle1 = 64*alpha;
	int angle2 = 64*arc;

	/* draw line */
	XDrawArc(windowDisplay->display, window, graphicsContext, 
			x, y, width, height, angle1, angle2);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::fillArc(int cx, int cy, int rx, int ry, int alpha, int arc, 
		unsigned long, unsigned long fillcolor)
{
	FTRC();
	/* convert to X-window arguments */
	int x = cx - rx;
	int y = cy - ry;
	unsigned int width = 2*rx;
	unsigned int height = 2*ry;
	int angle1 = 64*alpha;
	int angle2 = 64*arc;

	/* set foreground color */
	XSetForeground(windowDisplay->display, graphicsContext, fillcolor);

	/* draw line */
	XFillArc(windowDisplay->display, window, graphicsContext, 
			x, y, width, height, angle1, angle2);

	/* all done */
	status = OK;
	return(OK);
}

/* draw or fill a box */
int
XWindow::drawBox(int x0, int y0, int x1, int y1, unsigned long color)
{
	FTRC();
	/* convert to X-window arguments */
	unsigned int width = x1 - x0 + 1;
	unsigned int height = y1 - y0 + 1;

	/* set foreground color */
	XSetForeground(windowDisplay->display, graphicsContext, color);

	/* draw line */
	XDrawRectangle(windowDisplay->display, window, graphicsContext, 
			x0, y0, width, height);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::drawBox(int x0, int y0, int x1, int y1)
{
	FTRC();
	/* convert to X-window arguments */
	unsigned int width = x1 - x0 + 1;
	unsigned int height = y1 - y0 + 1;

	/* draw line */
	XDrawRectangle(windowDisplay->display, window, graphicsContext, 
			x0, y0, width, height);

	/* all done */
	status = OK;
	return(OK);
}

int
XWindow::fillBox(int x0, int y0, int x1, int y1, unsigned long, 
			unsigned long fillcolor)
{
	FTRC();
	/* convert to X-window arguments */
	unsigned int width = x1 - x0 + 1;
	unsigned int height = y1 - y0 + 1;

	/* set foreground color */
	XSetForeground(windowDisplay->display, graphicsContext, fillcolor);

	/* draw line */
	XFillRectangle(windowDisplay->display, window, graphicsContext, 
			x0, y0, width, height);

	/* all done */
	status = OK;
	return(OK);
}

/* display screen */
int
XWindow::display()
{
	FTRC();
	/* check if window is mapped */
	if ( ! mapped)
	{
		/* map window to display it */
		XMapRaised(windowDisplay->display, window);
		mapped = 1;

		/* flush buffer */
		XFlush(windowDisplay->display);

		/* check if any expose events */
		XEvent event;
		XCheckWindowEvent(windowDisplay->display, window, 
				ExposureMask, &event);
	}
	else
	{
		/* flush buffer */
		XFlush(windowDisplay->display);

		/* check if any expose events */
		XEvent event;
		XCheckWindowEvent(windowDisplay->display, window, 
				ExposureMask, &event);
	}

	/* all done */
	status = OK;
	return(OK);
}

