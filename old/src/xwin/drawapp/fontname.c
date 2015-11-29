/* headers */
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* get and set a font from argv */
XFontStruct *
LoadFontName(display, argc, argv, font_name, fallback_font_name)
Display *display;
int argc;
char **argv;
char *font_name;
char *fallback_font_name;
{
	char *ptr;
	XFontStruct *font_struct;

	/* find parameter */
	ptr = FindParameter(argc, argv, "-font");
	if (ptr != (char *)NULL)
	{
		strcpy(font_name, ptr);
	}
	else
	{
		ptr = FindParameter(argc, argv, "-fn");
		if (ptr != (char *)NULL)
		{
			strcpy(font_name, ptr);
		}
	}

	/* load font */
	font_struct = LoadFont(display, font_name, fallback_font_name);

	/* all done */
	return(font_struct);
}
