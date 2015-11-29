// headers 
#include "myxlib.h"

// load up a font
XFontStruct *
LoadFont(Display *display, char *font_name, char *fallback_font_name)
{
	// load up a font
	XFontStruct *font_struct = XLoadQueryFont(display, font_name);
	if (font_struct == (XFontStruct *)0)
	{
		font_struct = XLoadQueryFont(display, fallback_font_name);
		if (font_struct == NULL)
		{
			ERRORD("unable to load font.", font_name, EINVAL);
			ERRORD("unable to load font.", fallback_font_name, 
				EINVAL);
			XCloseDisplay(display);
			exit(2);
		}
	}

	// all done
	return(font_struct);
}
