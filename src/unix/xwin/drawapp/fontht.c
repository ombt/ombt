/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* determine height for text in a font */
int
FontHeight(font_struct)
XFontStruct *font_struct;
{
	/* return height */
	return(font_struct->ascent + font_struct->descent);
}
