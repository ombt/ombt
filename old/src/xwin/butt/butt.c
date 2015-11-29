// graph a set of data using x-windows

// headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Xlib headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// local headers
#include "returns.h"
#include "debug.h"

// local definitions
#if 0
#define ButtonsPerRow 3
#define ButtonsPerColumn 4
#else
#define ButtonsPerRow 8
#define ButtonsPerColumn 5
#endif
#define NumberOfButtons (ButtonsPerRow*ButtonsPerColumn)
#define VerticalSpace 10
#define HorizontalSpace 10
#define Space 20

// event masks for main window and buttons
#define EventMask \
	(ExposureMask | \
	 StructureNotifyMask | \
	 KeyPressMask | \
	 ButtonPressMask)
#define ButtonEventMask \
	(ExposureMask | \
	ButtonPressMask)

// globals for options
char iconic[BUFSIZ] = "NO";
char display[BUFSIZ] = "";
char bordercolor[BUFSIZ] = "black";
char foreground[BUFSIZ] = "black";
char background[BUFSIZ] = "white";
char title[BUFSIZ] = "buttons";
char name[BUFSIZ] = "buttons";
char geometry[BUFSIZ] = "";
char font[BUFSIZ] = "fixed";

// global for information abut button windows
struct ButtWinInfo {
	Window window;
	int off;
	int x, y;
	int width, height;
} buttwind[NumberOfButtons];

// usage message
void
usage(const char *cmd)
{
	printf("usage: %s [standard X options]\n", cmd);
	return;
}

// get command lines options
int 
cmdopts(int argc, char **argv, int &nextarg)
{
	// get command line option
	for (nextarg = 1; nextarg < argc; nextarg++)
	{
		if (strcmp(argv[nextarg], "-display") == 0)
		{
			strcpy(display, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-bordercolor") == 0) ||
			 (strcmp(argv[nextarg], "-bd") == 0))
		{
			strcpy(bordercolor, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-foreground") == 0) ||
			 (strcmp(argv[nextarg], "-fg") == 0))
		{
			strcpy(foreground, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-background") == 0) ||
			 (strcmp(argv[nextarg], "-bg") == 0))
		{
			strcpy(background, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-title") == 0)
		{
			strcpy(title, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-name") == 0)
		{
			strcpy(name, argv[++nextarg]);
		}
		else if (strcmp(argv[nextarg], "-iconic") == 0)
		{
			strcpy(name, "YES");
		}
		else if ((strcmp(argv[nextarg], "-help") == 0) ||
			 (strcmp(argv[nextarg], "-?") == 0))
		{
			usage(argv[0]);
			exit(0);
		}
		else if ((strcmp(argv[nextarg], "-geometry") == 0) ||
			 (strcmp(argv[nextarg], "-geom") == 0))
		{
			strcpy(geometry, argv[++nextarg]);
		}
		else if ((strcmp(argv[nextarg], "-font") == 0) ||
			 (strcmp(argv[nextarg], "-fn") == 0))
		{
			strcpy(font, argv[++nextarg]);
		}
		else if (strncmp(argv[nextarg], "-", 1) == 0)
		{
			// unknown option. error.
			return(NOTOK);
		}
		else
		{
			// finished with options
			break;
		}
	}

	// all done
	return(OK);
}

// open and close connections to display
int
ConnectToServer(char *display, Display *&xdisplay)
{
	// check display variable
	if (display == NULL || *display == 0)
	{
		display = getenv("DISPLAY");
	}

	// open x server
	if ((xdisplay = XOpenDisplay(display)) == NULL)
	{
		fprintf(stderr, "cannot connect to X server %s.\n",
			XDisplayName(display));
		return(NOTOK);
	}

	// all done
	return(OK);
}

void
DisconnectFromServer(Display *xdisplay)
{
	XCloseDisplay(xdisplay);
}

// get data for physical display
int
GetDisplayData(Display *xdisplay, int &xscreen, 
	unsigned int &display_width, unsigned int &display_height)
{
	// default physical screen attributes
	xscreen = DefaultScreen(xdisplay);
	display_width = DisplayWidth(xdisplay, xscreen);
	display_height = DisplayHeight(xdisplay, xscreen);

	// all done
	return(OK);
}

// load up a font
XFontStruct *
LoadFont(Display *display, char *font_name, char *fallback_font_name)
{
	// load up a font
	XFontStruct *font_struct = XLoadQueryFont(display, font_name);
	if (font_struct == NULL)
	{
		font_struct = XLoadQueryFont(display, fallback_font_name);
		if (font_struct == NULL)
		{
			fprintf(stderr, "unable to load font.", 
				font_name);
			fprintf(stderr, "unable to load font.", 
				fallback_font_name);
			return(NULL);
		}
	}

	// all done
	return(font_struct);
}

// get size and location of all windows
int
GetWindowGeometry(XFontStruct *xfont, unsigned int dw, unsigned int dh,
	int &ww, int &wh, int &wx, int &wy, struct ButtWinInfo *bw)
{
	// number of characters to print: "ON" or "OFF"
	int maxchars = strlen("OFF");

	// get maximum width and height for font.
	int maxwidth = maxchars*xfont->max_bounds.width;
	int maxheight = xfont->max_bounds.ascent+xfont->max_bounds.descent;

	// space surronding character in button
	int vertical_space = VerticalSpace;
	int horizontal_space = HorizontalSpace;

	// space between buttons and to edge of window
	int space = Space;

	// default width of window for buttons in a row
	ww = (ButtonsPerRow+1)*space + 
		2*ButtonsPerRow*horizontal_space + ButtonsPerRow*maxwidth;
	
	// default height of window for buttons in a column
	wh = (ButtonsPerColumn+1)*space + 
		2*ButtonsPerColumn*vertical_space + ButtonsPerColumn*maxheight;

	// default origin for window
	wx = (dw - ww)/2;
	wy = (dh - wh)/2;

	// fill in button information
	for (int ib = 0; ib < NumberOfButtons; ib++)
	{
		// width and height of button window.
		bw[ib].width = 2*horizontal_space+maxwidth;
		bw[ib].height = 2*vertical_space+maxheight;

		// x coordinate for button
		int ir = ib%ButtonsPerRow;
		bw[ib].x = (ir+1)*space + ir*(2*horizontal_space+maxwidth);

		// y-coordinate for button
		int ic;
		if (ib%ButtonsPerRow == 0)
			ic = ib/ButtonsPerRow;
		bw[ib].y = (ic+1)*space + ic*(2*vertical_space+maxheight);
	}

	// get geometry that user gave.
	if (geometry != NULL && *geometry != 0)
	{
		// user specified some geometry
		int user_x, user_y;	
		unsigned int user_width, user_height;
		int status = XParseGeometry(geometry, &user_x, &user_y, 
					&user_width, &user_height);

		if (status & WidthValue && user_width > ww) 
			ww = user_width;
		if (status & HeightValue && user_height > wh) 
			wh = user_height;
		if (status & XValue) 
			wx = user_x;
		if (status & YValue) 
			wy = user_y;
		if (status & XNegative) 
			wx = dw - ww + wx;
		if (status & YNegative) 
			wy = dh - wh + wy;
	}

	// all done
	return(OK);
}

// set window size and location hints
int
SetSizeHints(Display *display, Window window, 
		int x, int y, int width, int height)
{
	XSizeHints sizehints;

	// fill size hints
	sizehints.x = x;
	sizehints.y = y;
	sizehints.height = height;
	sizehints.width = width;
	sizehints.min_height = height;
	sizehints.min_width = width;
	sizehints.flags = USPosition | USSize | PMinSize;
	sizehints.base_width = width;
	sizehints.base_height = height;
	sizehints.flags |= PBaseSize;

	// set size hints 
	XSetWMNormalHints(display, window, &sizehints);

	// all done
	return(OK);
}

// set window name using text properties
int
SetWindowName(Display *display, Window window, char *name)
{
	XTextProperty text_property;

	// convert character string to text property
	(void) XStringListToTextProperty(&name, 1, &text_property);

	// set window name
	(void) XSetWMName(display, window, &text_property);

	// all done
	return(OK);
}

// set class hints
int
SetClassHints(Display *display, Window window, char *res_name, char *res_class)
{
	XClassHint class_hints;

	// fill in class hints
	class_hints.res_class = res_class;
	class_hints.res_name = res_name;

	// set values
	XSetClassHint(display, window, &class_hints);

	// all done
	return(OK);
}

//
// set window manager hints. the flag initial state indicates
// whether we want the window to be iconized or not. the 
// possible values are: WithDrawnState (0), NormalState(1), 
// and IconicState(3).
//
int
SetWMHints(Display *display, Window window, int initial_state)
{
	XWMHints wm_hints;

	// which flags are filled in
	wm_hints.flags = InputHint | StateHint;

	// start as icon or window
	wm_hints.initial_state = initial_state;

	// we want input
	wm_hints.input = True;

	// set values
	XSetWMHints(display, window, &wm_hints);

	// all done */
	return(OK);
}

// set up visual info
int
GetPseudoColorVisual(Display *display, int screen, Visual *&visual, int &depth)
{
	XVisualInfo *visual_array;
	XVisualInfo visual_info_template;
	int status = NOTOK;
	int number_visuals;

	// look for a pseudo-color visual
	if (DefaultVisual(display, screen)->c_class == PseudoColor)
	{
		// found a pseudo-color visual
		visual = DefaultVisual(display, screen);
		depth = DefaultDepth(display, screen);
		status = OK;
	}
	else
	{
		// looking for a pseudo-color visual
		visual_info_template.c_class = PseudoColor;
		visual_info_template.screen = screen;
		visual_array = XGetVisualInfo(display, 
			(VisualClassMask | VisualScreenMask),
			&visual_info_template, &number_visuals);
		if ((number_visuals > 0) && (visual_array != NULL))
		{
			visual = visual_array[0].visual;
			depth = visual_array[0].depth;
			XFree(visual_array);
			status = OK;
		}
		else
		{
			visual = CopyFromParent;
			status = NOTOK;
		}
	}

	// all done
	return(status);
}

// open a window
Window
CreateWindow(Display *display, Window parent, 
	int x, int y, int width, int height,
	unsigned long bordercolor, unsigned long backcolor,
	unsigned long event_mask, Visual *visual,
	unsigned int border_width, int depth)
{
	Window window;
	XSetWindowAttributes attributes;
	unsigned long attr_mask;

	// set up window attributes 
	attributes.event_mask = event_mask;
	attributes.border_pixel = bordercolor;
	attributes.background_pixel = backcolor;
	attr_mask = CWEventMask | CWBackPixel | CWBorderPixel;

	// create window
	window = XCreateWindow(display, parent, x, y, width, height,
			border_width, depth, InputOutput,
			visual, attr_mask, &attributes);

	// all done 
	return(window);
}

// check for alternate keys
int
IsAltKey(XKeyEvent *event)
{
	if ((event->type == KeyPress) || (event->type == KeyRelease))
	{
		if (event->state & Mod4Mask)
		{
			return(1);
		}
	}
	return(0);
}

// check for quit
int
IsAltQ(XKeyEvent *event, char *string)
{
	int status;

	status = 0;
#if 0
	if (IsAltKey(event) == True)
	{
		if (*string == 'Q' || *string == 'q')
		{
			status = 1;
		}
	}
#else
	if (*string == 'Q' || *string == 'q')
	{
		status = 1;
	}
#endif
	return(status);
}

// decode a key-press event
int
DecodeKeyPress(XKeyEvent *event, KeySym *keysym, char *string)
{
	int length;

	// initialize
	*keysym = 0;
	*string = 0;

	// verify the event
	if ((event->type != KeyPress) && (event->type == KeyRelease))
	{
		return(0);
	}

	// convert event to a string
	length =  XLookupString(event, string, 1, keysym, NULL);
	if (length > 0)
		string[1] = 0;
	else
		string[0] = 0;
	
	// all done
	return(IsAltQ(event, string));
}

// set up color map for a screen
int
SetUpColorMap(Display *display, int screen, 
	Window window, Visual *visual, Colormap &colormap)
{
	int status = NOTOK;

	// look for a pseudo-color visual
	if (visual == DefaultVisual(display, screen))
	{
		colormap = DefaultColormap(display, screen);
		status = OK;
	}
	else
	{
		if (visual == CopyFromParent) 
			visual = DefaultVisual(display, screen);
		colormap = XCreateColormap(display, window, visual, AllocNone);
		if (colormap != None)
		{
			XSetWindowColormap(display, window, colormap);
			status = OK;
		}
		else
		{
			colormap = DefaultColormap(display, screen);
		}
	}

	// all done 
	return(status);
}

// set up color map for a screen
unsigned long
AllocateColorByName(Display *display, Colormap colormap, char *colorname, unsigned long default_color)
{
	XColor hardwarecolor, exactcolor;
	unsigned long color;
	int status;

	// look for given color
	status = XAllocNamedColor(display, colormap, colorname,
			&hardwarecolor, &exactcolor);
	if (status != 0)
	{
		color = hardwarecolor.pixel;
	}
	else
	{
		color = default_color;
	}

	/* all done */
	return(color);
}

// create a graphics context
GC
CreateGC(Display *display, Drawable drawable, XFontStruct *font, 
	unsigned long forecolor, unsigned long backcolor)
{
	XGCValues xgcvalues;
	GC gc;

	// set colors
	xgcvalues.foreground = forecolor;
	xgcvalues.background = backcolor;

	// set values
	gc = XCreateGC(display, drawable, (GCForeground | GCBackground),
			&xgcvalues);

	// set up font in GC
	XSetFont(display, gc, font->fid);

	// all done
	return(gc);
}

// create a GC for xor-drawing
GC
CreateXorGC(Display *display, Drawable drawable, XFontStruct *font,
	unsigned long forecolor, unsigned long backcolor)
{
	XGCValues xgcvalues;
	GC gc;

	// set colors
	xgcvalues.foreground = forecolor ^ backcolor;
	xgcvalues.background = 0;
	xgcvalues.function = GXxor;

	// set values
	gc = XCreateGC(display, drawable, 
			(GCFunction | GCForeground | GCBackground),
			&xgcvalues);

	// set up font in GC
	XSetFont(display, gc, font->fid);

	// all done
	return(gc);
}

// main entry point
main(int argc, char **argv)
{
	// get command line options
	int nextarg;
	if (cmdopts(argc, argv, nextarg) != OK)
	{
		fprintf(stderr, "invalid command line option [%s].\n",
			argv[nextarg]);
		return(2);
	}

	// init button data
	for (int ib = 0; ib < NumberOfButtons; ib++)
	{
		buttwind[ib].window = 0;
		buttwind[ib].off = 1;
		buttwind[ib].x = 0;
		buttwind[ib].y = 0;
		buttwind[ib].width = 0;
		buttwind[ib].height = 0;
	}

	// connect to server
	Display *xdisplay = NULL;
	if (ConnectToServer(display, xdisplay) != OK)
	{
		fprintf(stderr, "unable to open display to server %s.\n",
			XDisplayName(display));
		return(2);
	}

	// get physical screen attributes
	int xscreen;
	unsigned int xdisplay_width, xdisplay_height;
	if (GetDisplayData(xdisplay, xscreen, 
			xdisplay_width, xdisplay_height) != OK)
	{
		fprintf(stderr, "unable to get display attributes.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// get font for writing
	XFontStruct *xfont = LoadFont(xdisplay, font, "fixed");
	if (xfont == NULL)
	{
		fprintf(stderr, "unable to load font.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// determine window sizes and locations
	int window_width, window_height, window_x, window_y;
	if (GetWindowGeometry(xfont, xdisplay_width, xdisplay_height, 
			window_width, window_height, 
			window_x, window_y, buttwind) != OK)
	{
		fprintf(stderr, "unable to get window geometry.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}
	fprintf(stderr, "window width : %d\n", window_width);
	fprintf(stderr, "window height: %d\n", window_height);
	fprintf(stderr, "window origin: (%d,%d)\n", window_x, window_y);
	for (int ib = 0; ib < NumberOfButtons; ib++)
	{
		fprintf(stderr, "button [%d] (x,y,w,h) = (%d,%d,%d,%d)\n",
			ib,
			buttwind[ib].x, buttwind[ib].y,
			buttwind[ib].width, buttwind[ib].height);
	}

	// default black and white pixels
	unsigned long black = BlackPixel(xdisplay, xscreen);
	unsigned long white = WhitePixel(xdisplay, xscreen);

	// get pseudo-color display, if any
	int window_depth;
	Visual *window_visual;
	if (GetPseudoColorVisual(xdisplay, xscreen, 
			window_visual, window_depth) != OK)
	{
		fprintf(stderr, "unable to get a color visual.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// create a window
	unsigned int window_border_width = 4;
	Window root_window = RootWindow(xdisplay, xscreen);
window_visual=CopyFromParent;
window_depth=CopyFromParent;
	Window window = CreateWindow(
		xdisplay, root_window, 
		window_x, window_y, window_width, window_height, 
		black, white, EventMask,
		window_visual, window_border_width, window_depth);

	// set size hints
	if (SetSizeHints(xdisplay, window, 
		window_x, window_y, window_width, window_height) != OK)
	{
		fprintf(stderr, "unable to set window size hints.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// set window name
	if (SetWindowName(xdisplay, window, title) != OK)
	{
		fprintf(stderr, "unable to set window title.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// set class hints
	if (SetClassHints(xdisplay, window, argv[0], argv[0]) != OK)
	{
		fprintf(stderr, "unable to set class hints.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// set other window manager hints
	int initstate = 
		(strcmp(iconic, "YES") == 0) ? IconicState : NormalState;
	if (SetWMHints(xdisplay, window, initstate) != OK)
	{
		fprintf(stderr, "unable to set WM hints.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// set up color map for window
	Colormap window_colormap;
	if (SetUpColorMap(xdisplay, xscreen, 
			window, window_visual, window_colormap) != OK)
	{
		fprintf(stderr, "unable to set up a color map.\n");
		DisconnectFromServer(xdisplay);
		return(2);
	}

	// allocate colors for background and foreground
	unsigned long window_background = 
		AllocateColorByName(xdisplay, window_colormap, 
				background, white);
	unsigned long window_foreground = 
		AllocateColorByName(xdisplay, window_colormap, 
				foreground, black);

	// create GC for drawing
	GC gc = CreateGC(xdisplay, window, xfont, 
			window_foreground, window_background);

	// create button windows
	unsigned int button_border_width = 2;
	for (int ib = 0; ib < NumberOfButtons; ib++)
	{
		buttwind[ib].window = CreateWindow(
			xdisplay, window, 
			buttwind[ib].x, buttwind[ib].y, 
			buttwind[ib].width, buttwind[ib].height, 
			black, white, ButtonEventMask,
			CopyFromParent, button_border_width, CopyFromParent);
		if (buttwind[ib].window == (Window)None)
		{
			fprintf(stderr, "failed to create button [%d].\n", ib);
		}
		else
		{
			fprintf(stderr, "button [%d] window = %d\n",
				ib, buttwind[ib].window);
		}
	}

	// map window
	XMapRaised(xdisplay, window);
	XMapSubwindows(xdisplay, window);
	XFlush(xdisplay);

	// event loop
	KeySym keysym;
	char string[BUFSIZ];
	for (int done = 0; !done; )
	{
		XEvent event;
		XNextEvent(xdisplay, &event);
		switch (event.type)
		{
		case Expose:
			{
			for (int ib = 0; ib < NumberOfButtons; ib++)
			{
				if (buttwind[ib].window == 
						event.xbutton.window)
				{
					if (buttwind[ib].off)
					{
						XFillRectangle(
							xdisplay,
							buttwind[ib].window,
							gc,
							0, 0,
							buttwind[ib].width,
							buttwind[ib].height);
						XSetForeground(xdisplay, gc,
							white);
						XDrawString(
							xdisplay,
							buttwind[ib].window,
							gc,
							HorizontalSpace,
							VerticalSpace + 
							xfont->max_bounds.ascent,
							"OFF",
							strlen("OFF"));
						XSetForeground(xdisplay, gc,
							black);
					}
					else
					{
						XFillRectangle(
							xdisplay,
							buttwind[ib].window,
							gc,
							0, 0,
							buttwind[ib].width,
							buttwind[ib].height);
						XSetForeground(xdisplay, gc,
							white);
						XDrawString(
							xdisplay,
							buttwind[ib].window,
							gc,
							HorizontalSpace,
							VerticalSpace + 
							xfont->max_bounds.ascent,
							"ON",
							strlen("ON"));
						XSetForeground(xdisplay, gc,
							black);
					}
					XFlush(xdisplay);
					break;
				}
			}
			}
			break;
		case ConfigureNotify:
			window_x = event.xconfigure.x;
			window_y = event.xconfigure.y;
			window_width = event.xconfigure.width;
			window_height = event.xconfigure.height;
			break;
		case KeyPress:
			done = DecodeKeyPress(&event.xkey, &keysym, string);
			break;
		case MappingNotify:
			XRefreshKeyboardMapping(&event.xmapping);
			break;
		case ButtonPress:
			{
			for (int ib = 0; ib < NumberOfButtons; ib++)
			{
				if (buttwind[ib].window == 
						event.xbutton.window)
				{
					if (buttwind[ib].off)
					{
						XFillRectangle(
							xdisplay,
							buttwind[ib].window,
							gc,
							0, 0,
							buttwind[ib].width,
							buttwind[ib].height);
						XSetForeground(xdisplay, gc,
							white);
						XDrawString(
							xdisplay,
							buttwind[ib].window,
							gc,
							HorizontalSpace,
							VerticalSpace + 
							xfont->max_bounds.ascent,
							"ON",
							strlen("ON"));
						XSetForeground(xdisplay, gc,
							black);
						buttwind[ib].off = 0;
					}
					else
					{
						XFillRectangle(
							xdisplay,
							buttwind[ib].window,
							gc,
							0, 0,
							buttwind[ib].width,
							buttwind[ib].height);
						XSetForeground(xdisplay, gc,
							white);
						XDrawString(
							xdisplay,
							buttwind[ib].window,
							gc,
							HorizontalSpace,
							VerticalSpace + 
							xfont->max_bounds.ascent,
							"OFF",
							strlen("OFF"));
						XSetForeground(xdisplay, gc,
							black);
						buttwind[ib].off = 1;
					}
					XFlush(xdisplay);
					break;
				}
			}
			fprintf(stderr, 
			"received ButtonPress [%d] for window [%d]\n",
			event.xbutton.button, event.xbutton.window);
			}
			break;
		}
	}

	// close connection to server
	DisconnectFromServer(xdisplay);

	// all done
	return(0);
}

