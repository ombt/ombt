/* display a spectrum */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* X-windows headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "complex.h"
#include "spectrum.h"

/* local definitions */
#define LOGPLOT 1
#define LOG10PLOT 2
#define LINPLOT 4
#define SQRTPLOT 8
#define REALVAL 16
#define IMAGVAL 32
#define ABSVAL 64
#define MAXCHAN 32767
#define FLUSHBUFFER 5

/* error codes */
static char *ErrorCodes[] = {
	"",
	"CreateWindow",
	"ChangeWindowAttributes",
	"GetWindowAttributes",
	"DestroyWindow",
	"DestroySubwindows",
	"ChangeSaveSet",
	"ReparentWindow",
	"MapWindow",
	"MapSubwindows",
	"UnmapWindow",
	"UnmapSubwindows",
	"ConfigureWindow",
	"CirculateWindow",
	"GetGeometry",
	"QueryTree",
	"InternAtom",
	"GetAtomName",
	"ChangeProperty",
	"DeleteProperty",
	"GetProperty",
	"ListProperties",
	"SetSelectionOwner",
	"GetSelectionOwner",
	"ConvertSelection",
	"SendEvent",
	"GrabPointer",
	"UngrabPointer",
	"GrabButton",
	"UngrabButton",
	"ChangeActivePointerGrab",
	"GrabKeyboard",
	"UngrabKeyboard",
	"GrabKey",
	"UngrabKey",
	"AllowEvents",
	"GrabServer",
	"UngrabServer",
	"QueryPointer",
	"GetMotionEvents",
	"TranslateCoords",
	"WarpPointer",
	"SetInputFocus",
	"GetInputFocus",
	"QueryKeymap",
	"OpenFont",
	"CloseFont",
	"QueryFont",
	"QueryTextExtents",
	"ListFonts",
	"ListFontsWithInfo",
	"SetFontPath",
	"GetFontPath",
	"CreatePixmap",
	"FreePixmap",
	"CreateGC",
	"ChangeGC",
	"CopyGC",
	"SetDashes",
	"SetClipRectangles",
	"FreeGC",
	"ClearArea",
	"CopyArea",
	"CopyPlane",
	"PolyPoint",
	"PolyLine",
	"PolySegment",
	"PolyRectangle",
	"PolyArc",
	"FillPoly",
	"PolyFillRectangle",
	"PolyFillArc",
	"PutImage",
	"GetImage",
	"PolyText8",
	"PolyText16",
	"ImageText8",
	"ImageText16",
	"CreateColormap",
	"FreeColormap",
	"CopyColormapAndFree",
	"InstallColormap",
	"UninstallColormap",
	"ListInstalledColormaps",
	"AllocColor",
	"AllocNamedColor",
	"AllocColorCells",
	"AllocColorPlanes",
	"FreeColors",
	"StoreColors",
	"StoreNamedColor",
	"QueryColors",
	"LookupColor",
	"CreateCursor",
	"CreateGlyphCursor",
	"FreeCursor",
	"RecolorCursor",
	"QueryBestSize",
	"QueryExtension",
	"ListExtensions",
	"ChangeKeyboardMapping",
	"GetKeyboardMapping",
	"ChangeKeyboardControl",
	"GetKeyboardControl",
	"Bell",
	"ChangePointerControl",
	"GetPointerControl",
	"SetScreenSaver",
	"GetScreenSaver",
	"ChangeHosts",
	"ListHosts",
	"SetAccessControl",
	"SetCloseDownMode",
	"KillClient",
	"RotateProperties",
	"ForceScreenSaver",
	"SetPointerMapping",
	"GetPointerMapping",
	"SetModifierMapping",
	"GetModifierMapping",
	"NoOperation"
};

/* globals */
extern int optind;
extern char *optarg;

/* usage msg */
void
usage(char *cmd)
{
	fprintf(stderr, 
	"usage: %s [-?] [-c minch,maxch] [-r minval,maxval]\n"
	"      [-d real|imag|abs] [-t linear|log|log10|sqrt]\n"
	"      [-g widthxheight[-+]xoff[-+]yoff] spectrum\n",
		cmd);
	return;
}

/* generic error handler */
int
ErrorHandler(Display *display, XErrorEvent *event)
{
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
	return(2);
}

/* fatal error handler */
int
FatalErrorHandler(Display *display)
{
	/* convert to a text string and print */
	fprintf(stderr, "X error: fatal IO error on display %s.\n",
		DisplayString(display));
	exit(2);
	return(NOTOK);
}

/* display spectrum using X-windows */
int
displaySpectrum(Spectrum &sp, int flags, double minval, double maxval,
		int minch, int maxch, char *geometry, char *specfile)
{
	/* for now */
	minch = minch;
	maxch = maxch;
	minval = minval;
	maxval = maxval;

	/* connect to X-server */
	Display *display = XOpenDisplay((char *)0);
	if (display == (Display *)0)
	{
		ERRORS("unable to connect to X-server.", 
			XDisplayName((char *)0), EINVAL);
		return(NOTOK);
	}

	/* set up error handlers */
	XSetErrorHandler(ErrorHandler);
	XSetIOErrorHandler(FatalErrorHandler);
	
	/* get screen number and root window id */
	int screen = DefaultScreen(display);
	Window rootwindow = RootWindow(display, screen);

	/* window measurements */
	int x = 10;
	int y = 10;
	int width = 400;
	int height = 250;
	int border_width = 2;
	int x1, y1;
	unsigned int width1, height1;

	/* parse geometry string */
	if (geometry != (char *)0)
	{
		int status = XParseGeometry(geometry, &x1, &y1, &width1, 
						&height1);
		if (status & XValue) x = x1;
		if (status & YValue) y = y1;
		if (status & WidthValue) width = width1;
		if (status & HeightValue) height = height1;
		if (status & XNegative) 
			x = DisplayWidth(display, screen) - width + x;
		if (status & YNegative) 
			y = DisplayHeight(display, screen) - height + y;
	}

	/* window attributes */
	XSetWindowAttributes winattr;
	winattr.event_mask = ExposureMask | KeyPressMask;
	winattr.border_pixel = BlackPixel(display, screen);
	winattr.background_pixel = WhitePixel(display, screen);
	unsigned long winattr_mask = CWEventMask | CWBackPixel | CWBorderPixel;
	Visual *visual = CopyFromParent;

	/* create window */
	Window window = XCreateWindow(display, rootwindow, 
				x, y, width, height, border_width,
				(int)CopyFromParent, InputOutput, visual,
				winattr_mask, &winattr);

	/* set size hints for window */
	XSizeHints sizehints;
	sizehints.x = x;
	sizehints.y = y;
	sizehints.width = width;
	sizehints.height = height;
	sizehints.min_width = width;
	sizehints.min_height = height;
	sizehints.base_width = width;
	sizehints.base_height = height;
	sizehints.flags = USPosition | USSize | PMinSize | PBaseSize;
	XSetWMNormalHints(display, window, &sizehints);

	/* store window name */
	XStoreName(display, window, specfile);

	/* store window-manager hints */
	XWMHints wmhints;
	wmhints.flags = InputHint | StateHint;
	wmhints.initial_state = NormalState;
	wmhints.input = True;
	XSetWMHints(display, window, &wmhints);

	/* create graphics context for drawing */
	XGCValues xgcvals;
	unsigned long xgcvals_mask = GCForeground | GCBackground;
	xgcvals.foreground = BlackPixel(display, screen);
	xgcvals.background = WhitePixel(display, screen);
	GC gc = XCreateGC(display, window, xgcvals_mask, &xgcvals);

	/* type of data to plot */
	if ((flags & REALVAL) == REALVAL)
	{
		for (int ich = 0; ich < sp.getNumChan(); ich++)
		{
			sp[ich] = real(sp[ich]);
		}
	}
	else if ((flags & IMAGVAL) == IMAGVAL)
	{
		for (int ich = 0; ich < sp.getNumChan(); ich++)
		{
			sp[ich] = imag(sp[ich]);
		}
	}
	else if ((flags & ABSVAL) == ABSVAL)
	{
		for (int ich = 0; ich < sp.getNumChan(); ich++)
		{
			sp[ich] = abs(sp[ich]);
		}
	}
	else
	{
		errno = EINVAL;
		ERROR("invalid type of data.", errno);
		return(NOTOK);
	}
	
	/* get type of plot */
	if ((flags & LOGPLOT) == LOGPLOT)
	{
		for (int ich = 0; ich < sp.getNumChan(); ich++)
		{
			if (sp[ich] != 0.0)
				sp[ich] = log(sp[ich]);
			else
				sp[ich] = 0.0;
		}
	}
	else if ((flags & LOG10PLOT) == LOG10PLOT)
	{
		for (int ich = 0; ich < sp.getNumChan(); ich++)
		{
			if (sp[ich] != 0.0)
				sp[ich] = log10(sp[ich]);
			else
				sp[ich] = 0.0;
		}
	}
	else if ((flags & SQRTPLOT) == SQRTPLOT)
	{
		for (int ich = 0; ich < sp.getNumChan(); ich++)
		{
			sp[ich] = sqrt(sp[ich]);
		}
	}
	else if ((flags & LINPLOT) != LINPLOT)
	{
		errno = EINVAL;
		ERROR("invalid type of plot.", errno);
		return(NOTOK);
	}

	/* find maximum and minimum values to plot */
	double miny = real(sp[0]);
	double maxy = real(sp[0]);
	for (int ich = 1; ich < sp.getNumChan(); ich++)
	{
		if (real(sp[ich]) < miny) miny = real(sp[ich]);
		if (real(sp[ich]) > maxy) maxy = real(sp[ich]);
	}
	double dely = maxy - miny;
	double minx = 0.0;
	double maxx = (double)sp.getNumChan() - 1;
	double delx = maxx - minx;
	int gxorig = 10;
	int gyorig = 10;
	int gdelx = width - 20;
	int gdely = height - 20;
	double gminx = gxorig;
	double gmaxx = gxorig + gdelx;
	double gminy = gyorig;
	double gmaxy = gyorig + gdely;

	/* make window appear */
	XMapRaised(display, window);
	XFlush(display);

	/* get into event loop */
	for (int done = 0; ! done; )
	{
		/* get an event */
		XEvent event;
		XNextEvent(display, &event);

		/* process event */
		switch (event.type)
		{
		case Expose:
		{
			/* clear window */
			XClearWindow(display, window);

			/* draw coordinate system */
			XDrawRectangle(display, window, gc, 
				       gxorig, gyorig, gdelx, gdely);
			XFlush(display);

			/* draw data */
			int oldgx = gxorig;
			int oldgy = gyorig;
			for (int ich = 0; ich < sp.getNumChan(); ich++)
			{
				/* get point's coordinates */
				double x = (double)ich;
				double y = real(sp[ich]);
		
				/* get screen coordinates */
				int gx = int(gminx + 
					(gmaxx - gminx)*(x - minx)/delx);
				int gy = int(gmaxy + 
					(gminy - gmaxy)*(y - miny)/dely);
		
				/* draw a line */
				XDrawLine(display, window, gc, oldgx, oldgy, 
					  gx, gy);

				/* save present coordinates */
				oldgx = gx;
				oldgy = gy;

				/* flush buffer */
				if (ich % FLUSHBUFFER == 0) XFlush(display);
			}
			XDrawLine(display, window, gc, oldgx, oldgy, 
				  (int)gmaxx, (int)gmaxy);
			XFlush(display);
		}
			break;

		case KeyPress:
		{
			/* exit */
			done = 1;
			break;
		}
		}
	}

	/* close display */
	XCloseDisplay(display);

	/* all done */
	return(OK);
}

/* main entry */
submain(int argc, char **argv)
{
	int c;
	char *p;
	char *geometry = (char *)0;
	int flags = LINPLOT | REALVAL;
	double minval = 0.0;
	double maxval = 1.0e38;
	int minch = 0;
	int maxch = MAXCHAN-1;

	/* get command line options */
	while ((c = getopt(argc, argv, "?c:r:d:t:g:")) != EOF)
	{
		/* get options */
		switch (c)
		{
		case 'c':
			/* get start and end channel */
			if ((p = strtok(optarg, ",")) != (char *)0)
			{
				minch = atoi(p);
				if ((p = strtok((char *)0, ",")) != (char *)0)
				{
					maxch = atoi(p);
				}
			}
			if (minch > maxch)
			{
				int tmp = minch;
				minch = maxch;
				maxch = tmp;
			}
			break;

		case 'r':
			/* get range of values */
			if ((p = strtok(optarg, ",")) != (char *)0)
			{
				sscanf(p, "%le", &minval);
				if ((p = strtok((char *)0, ",")) != (char *)0)
				{
					sscanf(p, "%le", &maxval);
				}
			}
			if (minval > maxval)
			{
				double tmp = minval;
				minval = maxval;
				maxval = tmp;
			}
			break;

		case 't':
			/* type of plot */
			if (strcmp(optarg, "log") == 0)
			{
				flags |= LOGPLOT;
				flags &= ~(LOG10PLOT|LINPLOT|SQRTPLOT);
			}
			else if (strcmp(optarg, "log10") == 0)
			{
				flags |= LOG10PLOT;
				flags &= ~(LOGPLOT|LINPLOT|SQRTPLOT);
			}
			else if (strcmp(optarg, "linear") == 0)
			{
				flags |= LINPLOT;
				flags &= ~(LOGPLOT|LOG10PLOT|SQRTPLOT);
			}
			else if (strcmp(optarg, "sqrt") == 0)
			{
				flags |= SQRTPLOT;
				flags &= ~(LOGPLOT|LOG10PLOT|LINPLOT);
			}
			else
			{
				ERROR("invalid option for -t.", EINVAL);
				usage(argv[0]);
				return(2);
			}
			break;

		case 'd':
			/* type of data to plot */
			if (strcmp(optarg, "real") == 0)
			{
				flags |= REALVAL;
				flags &= ~(IMAGVAL|ABSVAL);
			}
			else if (strcmp(optarg, "imag") == 0)
			{
				flags |= IMAGVAL;
				flags &= ~(REALVAL|ABSVAL);
			}
			else if (strcmp(optarg, "abs") == 0)
			{
				flags |= ABSVAL;
				flags &= ~(IMAGVAL|REALVAL);
			}
			else
			{
				ERROR("invalid option for -d.", EINVAL);
				usage(argv[0]);
				return(2);
			}
			break;
		case 'g':
			/* get new geometry */
			geometry = optarg;
			break;

		case '?':
			/* help message */
			usage(argv[0]);
			return(0);

		default:
			/* error */
			ERROR("invalid option.", EINVAL);
			usage(argv[0]);
			return(2);
		}
	}

	/* check if spectrum file was given */
	if ((optind + 1) > argc)
	{
		ERROR("missing spectrum file.", EINVAL);
		usage(argv[0]);
		return(2);
	}

	/* create a spectrum */
	Spectrum sp(argv[optind]);
	if ((errno = sp.getStatus()) != OK)
	{
		ERRORS("spectrum constructor failed for spectrum.", 
			argv[optind], errno);
		exit(2);
	}

	/* display spectrum */
	if (displaySpectrum(sp, flags, minval, maxval, minch, maxch, 
				geometry, argv[optind]) != OK)
	{
		ERRORS("unable to display spectrum.", argv[optind], errno);
		exit(2);
	}

	/* all done */
	return(0);
}

main(int argc, char **argv)
{
	exit(submain(argc, argv));
}

