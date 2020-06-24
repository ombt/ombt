// headers
#include "myxlib.h"

// determine height for text in a font
int
FontHeight(XFontStruct *font_struct)
{
	/* return height */
	return(font_struct->ascent + font_struct->descent);
}
