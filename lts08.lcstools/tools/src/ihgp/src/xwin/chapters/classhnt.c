// headers
#include "myxlib.h"

// set class hints
void
SetClassHints(Display *display, Window window, char *res_name, char *res_class)
{
	XClassHint class_hints;

	// fill in class hints
	class_hints.res_class = res_class;
	class_hints.res_name = res_name;

	// set values
	XSetClassHint(display, window, &class_hints);

	// all done
	return;
}
