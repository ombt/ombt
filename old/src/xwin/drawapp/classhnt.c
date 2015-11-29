/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* set class hints */
void
SetClassHints(display, window, res_name, res_class)
Display *display;
Window window;
char *res_name, *res_class;
{
	XClassHint class_hints;

	/* fill in class hints */
	class_hints.res_class = res_class;
	class_hints.res_name = res_name;

	/* set values */
	XSetClassHint(display, window, &class_hints);

	/* all done */
	return;
}
