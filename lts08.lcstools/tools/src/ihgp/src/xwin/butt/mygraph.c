// graph a set of data using x-windows

// headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>

// Xlib headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// local headers
#include "returns.h"
#include "debug.h"

// local definitions
#define StartOfOptions "++++ START OPTIONS ++++"
#define StartOfData "++++ START DATA ++++"
#define MinimumXValue "MINIMUM X VALUE"
#define MaximumXValue "MAXIMUM X VALUE"
#define MinimumYValue "MINIMUM Y VALUE"
#define MaximumYValue "MAXIMUM Y VALUE"
#define XAxisPlotType "X-AXIS PLOT TYPE"
#define YAxisPlotType "Y-AXIS PLOT TYPE"
#define XAxisLabel "X-AXIS LABEL"
#define YAxisLabel "Y-AXIS LABEL"
#define GraphTitle "GRAPH TITLE"
#define EventMask \
	(ExposureMask | \
	 StructureNotifyMask | \
	 KeyPressMask | \
	 ButtonPressMask | \
	 ButtonReleaseMask | \
	 ButtonMotionMask)

//  globals for options
char iconic[BUFSIZ] = "NO";
char display[BUFSIZ] = "";
char bordercolor[BUFSIZ] = "black";
char foreground[BUFSIZ] = "black";
char background[BUFSIZ] = "white";
char title[BUFSIZ] = "graph";
char name[BUFSIZ] = "graph";
char geometry[BUFSIZ] = "";
char font[BUFSIZ] = "fixed";

// globals for data
double rminxval = 0.0;
double rmaxxval = 0.0;
double rminyval = 0.0;
double rmaxyval = 0.0;
char minxval[BUFSIZ] = "";
char maxxval[BUFSIZ] = "";
char minyval[BUFSIZ] = "";
char maxyval[BUFSIZ] = "";
char xplottype[BUFSIZ] = "LINEAR";
char yplottype[BUFSIZ] = "LINEAR";
char xlabel[BUFSIZ] = "X-AXIS";
char ylabel[BUFSIZ] = "Y-AXIS";
char graph_title[BUFSIZ] = "NO TITLE";
double *x, *y;
int npts = 0;
int allocpts = 100;
int savexorig = -1;
int saveyorig = -1;
double reset_rminxval = 0.0;
double reset_rmaxxval = 0.0;
double reset_rminyval = 0.0;
double reset_rmaxyval = 0.0;

// usage message
void
usage(const char *cmd)
{
	printf("usage: %s [standard X options] [filename]\n", cmd);
	return;
}

// specialized string functions
int
mystrncmp(char *string1, char *string2)
{
	return(strncmp(string1, string2, strlen(string2)));
}

int
mystrncpy(char *string1, char *string2, char *prefix)
{
	// length of prefix
	int plen = strlen(prefix);

	// is string longer than prefix
	if (strlen(string2) <= plen)
	{
		*string1 = 0;
		return(OK);
	}

	// skip prefix and blank spaces
	string2 += strlen(prefix);
	for ( ; 
		(*string2 == ' ' || *string2 == '\t') && *string2 != 0; 
		string2++) ;

	// check if anything left
	if (*string2 == 0)
	{
		*string1 = 0;
		return(OK);
	}

	// copy remaining string
	strcpy(string1, string2);

	// remove trailing newline
	int s1len = strlen(string1);
	if (string1[s1len-1] == '\n')
		string1[s1len-1] = 0;

	// all done
	return(OK);
}

// get min or max values
void
getmin(double *dbls, int ndbls, double &mindbl, char *cmindbl)
{
	if (*cmindbl != 0)
		mindbl= strtod(cmindbl, NULL);
	else
	{
		mindbl = dbls[0];
		for (int ipt = 1; ipt < ndbls; ipt++)
		{
			if (dbls[ipt] < mindbl) 
				mindbl = dbls[ipt];
		}
	}
	return;
}

void
getmax(double *dbls, int ndbls, double &maxdbl, char *cmaxdbl)
{
	if (*cmaxdbl != 0)
		maxdbl= strtod(cmaxdbl, NULL);
	else
	{
		maxdbl = dbls[0];
		for (int ipt = 1; ipt < ndbls; ipt++)
		{
			if (dbls[ipt] > maxdbl) 
				maxdbl = dbls[ipt];
		}
	}
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

// read in data
int
getdata(int argc, char **argv, int nextarg)
{
	FILE *infd;
	char inbuf[BUFSIZ+1];

	// where to get data?
	if (nextarg >= argc)
	{
		// get data from stdin
		infd = stdin;
	}
	else
	{
		// get data from a file
		infd = fopen(argv[nextarg], "r");
		if (infd == NULL)
		{
			fprintf(stderr, "unable to read file %s.\n", 
				argv[nextarg]);
			return(NOTOK);
		}
	}

	// read in data: first any options
	while (fgets(inbuf, BUFSIZ, infd) != NULL && !feof(infd))
	{
		if (*inbuf == '#')
		{
			// skip comments
			continue;
		}
		else if (mystrncmp(inbuf, StartOfOptions) == 0)
		{
			continue;
		}
		else if (mystrncmp(inbuf, StartOfData) == 0)
		{
			// break out of options loop
			break;
		}
		else if (mystrncmp(inbuf, MinimumXValue) == 0)
		{
			mystrncpy(minxval, inbuf, MinimumXValue);
		}
		else if (mystrncmp(inbuf, MaximumXValue) == 0)
		{
			mystrncpy(maxxval, inbuf, MaximumXValue);
		}
		else if (mystrncmp(inbuf, MinimumYValue) == 0)
		{
			mystrncpy(minyval, inbuf, MinimumYValue);
		}
		else if (mystrncmp(inbuf, MaximumYValue) == 0)
		{
			mystrncpy(maxyval, inbuf, MaximumYValue);
		}
		else if (mystrncmp(inbuf, XAxisPlotType) == 0)
		{
			mystrncpy(xplottype, inbuf, XAxisPlotType);
		}
		else if (mystrncmp(inbuf, YAxisPlotType) == 0)
		{
			mystrncpy(yplottype, inbuf, YAxisPlotType);
		}
		else if (mystrncmp(inbuf, XAxisLabel) == 0)
		{
			mystrncpy(xlabel, inbuf, XAxisLabel);
		}
		else if (mystrncmp(inbuf, YAxisLabel) == 0)
		{
			mystrncpy(ylabel, inbuf, YAxisLabel);
		}
		else if (mystrncmp(inbuf, GraphTitle) == 0)
		{
			mystrncpy(graph_title, inbuf, GraphTitle);
		}
		else
		{
			// unknown option
			fprintf(stderr, "SKIPPING unknown option in graph [%s].\n",
				inbuf);
		}
	}

	// check if out of data
	if (feof(infd))
	{
		fprintf(stderr, "unexpected EOF. no data read in.\n");
		return(NOTOK);
	}

	// allocate space for a 100 points, first.
	x = new double[allocpts];
	MustBeTrue(x != NULL);
	y = new double[allocpts];
	MustBeTrue(y != NULL);

	// now read in data
	for (npts = 0; fgets(inbuf, BUFSIZ, infd) != NULL && !feof(infd); )
	{
		// read in x,y point
		double tmpx, tmpy;
		sscanf(inbuf, "%lg%lg", &tmpx, &tmpy);
	
		// store x,y point
		if (npts >= allocpts)
		{
			// need to reallocate 
			allocpts *= 2;
			double *px = new double [allocpts];
			MustBeTrue(px != NULL);
			double *py = new double [allocpts];
			MustBeTrue(py != NULL);
			for (int ipt = 0; ipt < npts; ipt++)
			{
				px[ipt] = x[ipt];
				py[ipt] = y[ipt];
			}
			delete [] x;
			delete [] y;
			x = px;
			y = py;
		}
		x[npts] = tmpx;
		y[npts++] = tmpy;
	}

	// find min and max limits for x values.
	getmin(x, npts, rminxval, minxval);
	getmax(x, npts, rmaxxval, maxxval);
	getmin(y, npts, rminyval, minyval);
	getmax(y, npts, rmaxyval, maxyval);

	// save values in case of reset
	reset_rminxval = rminxval;
	reset_rmaxxval = rmaxxval;
	reset_rminyval = rminyval;
	reset_rmaxyval = rmaxyval;

	// close file
	if (infd != stdin)
		fclose(infd);

	// all done
	return(OK);
}

// x-window functions
int
OpenDisplay(char *display, Display *&xdisplay)
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
CloseDisplay(Display *xdisplay)
{
	XCloseDisplay(xdisplay);
}

int
GetDisplayAttributes(Display *xdisplay, int &xscreen, 
	unsigned int &display_width, unsigned int &display_height)
{
	// default physical screen attributes
	xscreen = DefaultScreen(xdisplay);
	display_width = DisplayWidth(xdisplay, xscreen);
	display_height = DisplayHeight(xdisplay, xscreen);

	// all done
	return(OK);
}

int
GetWindowGeometry(unsigned int display_width, unsigned int display_height,
	int &window_width, int &window_height, int &window_x, int &window_y)
{
	// set default values
	window_x = display_width/8;
	window_y = display_height/8;
	window_width = display_width/3;
	window_height = display_height/3;

	// get geometry that user gave.
	if (*geometry != 0)
	{
		// user specified some geometry
		int user_x, user_y;	
		unsigned int user_width, user_height;
		int status = XParseGeometry(geometry, &user_x, &user_y, 
					&user_width, &user_height);
		if (status & XValue) window_x = user_x;
		if (status & YValue) window_y = user_y;
		if (status & WidthValue) window_width = user_width;
		if (status & HeightValue) window_height = user_height;
		if (status & XNegative)
			window_x = display_width - window_width + window_x;
		if (status & YNegative)
			window_y = display_height - window_height + window_y;
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
	if (IsAltKey(event) == True)
	{
		if (*string == 'Q' || *string == 'q')
		{
			status = 1;
		}
	}
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

// draw graph
void
redraw(Display *xdisplay, Window window, GC gc, XFontStruct *xfont, 
	unsigned int window_width, unsigned int window_height)
{
	// translate min and max labels to character strings
	char lxmin[BUFSIZ], lxmax[BUFSIZ];
	char lymin[BUFSIZ], lymax[BUFSIZ];
	sprintf(lxmin, "%-8lg", rminxval);
	sprintf(lxmax, "%8lg", rmaxxval);
	sprintf(lymin, "%8lg", rminyval);
	sprintf(lymax, "%8lg", rmaxyval);

	// get font height
	int font_height = xfont->ascent + xfont->descent;

	// get widths of each text string
	int lxminsz = XTextWidth(xfont, lxmin, strlen(lxmin));
	int lxmaxsz = XTextWidth(xfont, lxmax, strlen(lxmax));
	int lyminsz = XTextWidth(xfont, lymin, strlen(lymin));
	int lymaxsz = XTextWidth(xfont, lymax, strlen(lymax));

	// get x coordinate for origin
	int xorig = (lyminsz > lymaxsz) ? lyminsz : lymaxsz;
	xorig += 2;
	savexorig = xorig;

	// get y coordinate for origin
	int yorig = window_height - 3*font_height;
	yorig -= 2;
	saveyorig = yorig;

	// get maximum width for a character
	int maxwidth = xfont->max_bounds.width;

	// draw axes
	XDrawLine(xdisplay, window, gc, xorig, 0, xorig, yorig);
	XDrawLine(xdisplay, window, gc, xorig, yorig, window_width, yorig);
	XFlush(xdisplay);

	// draw x-axis min/max labels
	XDrawString(xdisplay, window, gc, 
		xorig, (window_height-2*font_height-1), 
		lxmin, strlen(lxmin));
	XDrawString(xdisplay, window, gc, 
		(window_width-lxmaxsz-1), (window_height-2*font_height-1), 
		lxmax, strlen(lxmax));
	XFlush(xdisplay);

	// draw y-axis min/max labels
	XDrawString(xdisplay, window, gc, 
		0, yorig-1,
		lymin, strlen(lymin));
	XDrawString(xdisplay, window, gc, 
		0, (font_height+1), 
		lymax, strlen(lymax));
	XFlush(xdisplay);

	// draw x-axis label, center it.
	char tmplabel[BUFSIZ];
	sprintf(tmplabel, "%s (%s)", xlabel, xplottype);
	int xcenter = xorig+(window_width-xorig)/2;
	int tmplabelsz = XTextWidth(xfont, tmplabel, strlen(tmplabel));
	XDrawString(xdisplay, window, gc, 
		(xcenter-tmplabelsz/2-1), (window_height-font_height-1), 
		tmplabel, strlen(tmplabel));

	// draw title, center it.
	sprintf(tmplabel, "%s", graph_title);
	tmplabelsz = XTextWidth(xfont, tmplabel, strlen(tmplabel));
	XDrawString(xdisplay, window, gc, 
		(xcenter-tmplabelsz/2-1), (window_height-1), 
		tmplabel, strlen(tmplabel));

	// draw y-axis label, center it.
	sprintf(tmplabel, "%s (%s)", ylabel, yplottype);
	int ycenter = yorig/2;
	tmplabelsz  = strlen(tmplabel);
	for (int iy = 0; iy < tmplabelsz; iy++)
	{
		XDrawString(xdisplay, window, gc, 
			(xorig-2*maxwidth-1), 
			(ycenter-font_height*tmplabelsz/2)+iy*font_height,
			(tmplabel+iy), 1);
	}

	// allocate arrays to hold point to plot
	double xptsmin, xptsmax, yptsmin, yptsmax;
	double *xpts = new double [npts];
	MustBeTrue(xpts != NULL);
	double *ypts = new double [npts];
	MustBeTrue(ypts != NULL);

	// calculate x,y logs, if necessary
	if (strcmp(xplottype, "LOG") == 0)
	{
		MustBeTrue(rminxval > 0.0 && rmaxxval > 0.0);
		xptsmin = log10(rminxval);
		xptsmax = log10(rmaxxval);
		for (int ipt = 0; ipt < npts; ipt++)
		{
			if (x[ipt] > 0.0)
				xpts[ipt] = log10(x[ipt]);
			else
				xpts[ipt] = xptsmin;
		}
	}
	else
	{
		for (int ipt = 0; ipt < npts; ipt++)
		{
			xpts[ipt] = x[ipt];
		}
		xptsmin = rminxval;
		xptsmax = rmaxxval;
	}
	if (strcmp(yplottype, "LOG") == 0)
	{
		MustBeTrue(rminyval > 0.0 && rmaxyval > 0.0);
		yptsmin = log10(rminyval);
		yptsmax = log10(rmaxyval);
		for (int ipt = 0; ipt < npts; ipt++)
		{
			if (y[ipt] > 0.0)
				ypts[ipt] = log10(y[ipt]);
			else
				ypts[ipt] = yptsmin;
		}
	}
	else
	{
		for (int ipt = 0; ipt < npts; ipt++)
		{
			ypts[ipt] = y[ipt];
		}
		yptsmin = rminyval;
		yptsmax = rmaxyval;
	}

	// draw lines
	for (int ipt = 0; ipt < (npts-1); ipt++)
	{
		// fix out-or-range points
		double xp1, yp1, xp2, yp2;
		if (xpts[ipt] < xptsmin)
			xp1 = xptsmin;
		else if (xpts[ipt] > xptsmax)
			xp1 = xptsmax;
		else
			xp1 = xpts[ipt];
		if (xpts[ipt+1] < xptsmin)
			xp2 = xptsmin;
		else if (xpts[ipt+1] > xptsmax)
			xp2 = xptsmax;
		else
			xp2 = xpts[ipt+1];
		if (ypts[ipt] < yptsmin)
			yp1 = yptsmin;
		else if (ypts[ipt] > yptsmax)
			yp1 = yptsmax;
		else
			yp1 = ypts[ipt];
		if (ypts[ipt+1] < yptsmin)
			yp2 = yptsmin;
		else if (ypts[ipt+1] > yptsmax)
			yp2 = yptsmax;
		else
			yp2 = ypts[ipt+1];

		// calculate coordinates
		int x1 = (xp1-xptsmin)*
			(window_width-xorig)/(xptsmax-xptsmin)+ xorig;
		int x2 = (xp2-xptsmin)*
			(window_width-xorig)/(xptsmax-xptsmin)+ xorig;
		int y1 = yorig*(1.0-(yp1-yptsmin)/(yptsmax-yptsmin));
		int y2 = yorig*(1.0-(yp2-yptsmin)/(yptsmax-yptsmin));

		// draw line
		XDrawLine(xdisplay, window, gc, x1, y1, x2, y2);
		XFlush(xdisplay);
	}

	// delete arrays
	delete [] xpts;
	delete [] ypts;

	// all done
	return;
}

// draw an item
void 
DrawBox(Display *display, Drawable drawable, GC gc, 
	int x1, int y1, int x2, int y2)
{
	int width = x2 - x1;
	if (width < 0) width = 0;
	int height = y2 - y1;
	if (height < 0) height = 0;
	XDrawRectangle(display, drawable, gc, x1, y1, width, height);
	return;
}

void
UpdateLimits(int x1, int y1, int x2, int y2, int width, int height)
{
	// correct any out of range values
	if (x1 < savexorig)
		x1 = savexorig;
	else if (x1 > width)
		x1 = width;
	if (x2 < savexorig)
		x2 = savexorig;
	else if (x2 > width)
		x2 = width;
	if (y1 < 0)
		y1 = 0;
	else if (y1 > saveyorig)
		y1 = saveyorig;
	if (y2 < 0)
		y2 = 0;
	else if (y2 > saveyorig)
		y2 = saveyorig;

	// calculate new x,y limits
	double newymin = 
		(saveyorig-y2)*(rmaxyval-rminyval)/saveyorig+rminyval;
	double newymax = 
		(saveyorig-y1)*(rmaxyval-rminyval)/saveyorig+rminyval;
	double newxmin = 
		(x1-savexorig)*(rmaxxval-rminxval)/(width-savexorig)+rminxval;
	double newxmax = 
		(x2-savexorig)*(rmaxxval-rminxval)/(width-savexorig)+rminxval;

	// update limits
	rminxval = newxmin;
	rmaxxval = newxmax;
	rminyval = newymin;
	rmaxyval = newymax;

	// all done
	return;
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

	// get data 
	if (getdata(argc, argv, nextarg) != OK)
	{
		fprintf(stderr, "unable to get data.");
		return(2);
	}

	// connect to server
	Display *xdisplay = NULL;
	if (OpenDisplay(display, xdisplay) != OK)
	{
		fprintf(stderr, "unable to open display to server %s.\n",
			XDisplayName(display));
		return(2);
	}

	// get physical screen attributes
	int xscreen;
	unsigned int display_width, display_height;
	if (GetDisplayAttributes(xdisplay, xscreen, 
			display_width, display_height) != OK)
	{
		fprintf(stderr, "unable to get display attributes.\n");
		CloseDisplay(xdisplay);
		return(2);
	}

	// determine window geometry and location
	int window_width, window_height, window_x, window_y;
	if (GetWindowGeometry(display_width, display_height, 
			window_width, window_height, 
			window_x, window_y) != OK)
	{
		fprintf(stderr, "unable to get window geometry.\n");
		CloseDisplay(xdisplay);
		return(2);
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
		CloseDisplay(xdisplay);
		return(2);
	}

	// create a window
	unsigned int window_border_width = 4;
	Window root_window = RootWindow(xdisplay, xscreen);
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
		CloseDisplay(xdisplay);
		return(2);
	}

	// set window name
	if (SetWindowName(xdisplay, window, title) != OK)
	{
		fprintf(stderr, "unable to set window title.\n");
		CloseDisplay(xdisplay);
		return(2);
	}

	// set class hints
	if (SetClassHints(xdisplay, window, argv[0], argv[0]) != OK)
	{
		fprintf(stderr, "unable to set class hints.\n");
		CloseDisplay(xdisplay);
		return(2);
	}

	// set other window manager hints
	int initstate = 
		(strcmp(iconic, "YES") == 0) ? IconicState : NormalState;
	if (SetWMHints(xdisplay, window, initstate) != OK)
	{
		fprintf(stderr, "unable to set WM hints.\n");
		CloseDisplay(xdisplay);
		return(2);
	}

	// set up color map for window
	Colormap window_colormap;
	if (SetUpColorMap(xdisplay, xscreen, 
			window, window_visual, window_colormap) != OK)
	{
		fprintf(stderr, "unable to set up a color map.\n");
		CloseDisplay(xdisplay);
		return(2);
	}

	// allocate colors for background and foreground
	unsigned long window_background = 
		AllocateColorByName(xdisplay, window_colormap, 
				background, white);
	unsigned long window_foreground = 
		AllocateColorByName(xdisplay, window_colormap, 
				foreground, black);

	// get font for writing
	XFontStruct *xfont = LoadFont(xdisplay, font, "fixed");
	if (xfont == NULL)
	{
		fprintf(stderr, "unable to load font.\n");
		CloseDisplay(xdisplay);
		return(2);
	}

	// create GC for drawing
	GC gc = CreateGC(xdisplay, window, xfont, 
			window_foreground, window_background);
	GC xor_gc = CreateXorGC(xdisplay, window, xfont, 
			window_foreground, window_background);

	// map window
	XMapRaised(xdisplay, window);
	XFlush(xdisplay);

	// event loop
	int start_x = -1;
	int start_y = -1;
	int last_x = -1;
	int last_y = -1;
	XEvent event;
	KeySym keysym;
	char string[BUFSIZ];
	for (int done = 0; !done; )
	{
		XNextEvent(xdisplay, &event);
		switch (event.type)
		{
		case Expose:
			if (event.xexpose.count == 0)
			{
				redraw(xdisplay, window, gc, xfont, 
					window_width, window_height);
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
			if (event.xbutton.button == Button1)
			{
				start_x = event.xbutton.x;
				start_y = event.xbutton.y;
				last_x = start_x;
				last_y = start_y;
				DrawBox(xdisplay, window, xor_gc, 
					start_x, start_y, last_x, last_y);
				XFlush(xdisplay);
			}
			break;
		case ButtonRelease:
			if (event.xbutton.button == Button1)
			{
				if (start_x != last_x && start_y != last_y)
				{
					DrawBox(xdisplay, window, xor_gc, 
						start_x, start_y, 
						last_x, last_y);
					XFlush(xdisplay);
					UpdateLimits(start_x, start_y, 
						last_x, last_y,
						window_width, window_height);
					XClearWindow(xdisplay, window);
					XFlush(xdisplay);
					redraw(xdisplay, window, gc, xfont, 
						window_width, window_height);
					XFlush(xdisplay);
				}
				start_x = -1;
				start_y = -1;
				last_x = -1;
				last_y = -1;
			}
			else 
			{
				// reset to original values.
				rminxval = reset_rminxval;
				rmaxxval = reset_rmaxxval;
				rminyval = reset_rminyval;
				rmaxyval = reset_rmaxyval;
				XClearWindow(xdisplay, window);
				XFlush(xdisplay);
				redraw(xdisplay, window, gc, xfont, 
					window_width, window_height);
				XFlush(xdisplay);
			}
			break;
		case MotionNotify:
			if (event.xbutton.button == Button1)
			{
				DrawBox(xdisplay, window, xor_gc, 
					start_x, start_y, last_x, last_y);
				last_x = event.xmotion.x;
				last_y = event.xmotion.y;
				DrawBox(xdisplay, window, xor_gc, 
					start_x, start_y, last_x, last_y);
				XFlush(xdisplay);
			}
			break;
		}
	}

	// close display
	CloseDisplay(xdisplay);

	// all done
	return(0);
}


