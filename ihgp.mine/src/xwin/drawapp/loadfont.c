/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* load up a font */
XFontStruct *
LoadFont(display, font_name, fallback_font_name)
Display *display;
char *font_name;
char *fallback_font_name;
{
	XFontStruct *font_struct;

	/* load up a font */
	font_struct = XLoadQueryFont(display, font_name);
	if (font_struct == (XFontStruct *)0)
	{
		font_struct = XLoadQueryFont(display, fallback_font_name);
		if (font_struct == (XFontStruct *)0)
		{
			ERRORS("unable to load font.", font_name, EINVAL);
			ERRORS("unable to load font.", fallback_font_name, 
				EINVAL);
			XCloseDisplay(display);
			exit(2);
		}
	}

	/* all done */
	return(font_struct);
}
