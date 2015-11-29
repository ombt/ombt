// headers
#include "myxlib.h"

// set window name
void
SetWindowName(Display *display, Window window, char *name)
{
	// set window name
	XStoreName(display, window, name);

	// all done
	return;
}

// set window name using text properties
void
SetWindowNameWithTextProps(Display *display, Window window, char *name)
{
	XTextProperty text_property;

	// convert character string to text property
	(void) XStringListToTextProperty(&name, 1, &text_property);

	// set window name
	(void) XSetWMName(display, window, &text_property);

	// all done
	return;
}
