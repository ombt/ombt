/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

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

/* set up error handlers */
void
SetErrorHandlers()
{
	XSetErrorHandler(ErrorHandler);
	XSetIOErrorHandler(FatalErrorHandler);
	return;
}

/* generic error handler */
int
ErrorHandler(display, event)
Display *display;
XErrorEvent *event;
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
	return(0);
}

/* fatal error handler */
int
FatalErrorHandler(display)
Display *display;
{
	/* convert to a text string and print */
	fprintf(stderr, "X error: fatal IO error on display %s.\n",
		DisplayString(display));
	exit(2);
}

