/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* set window manager hints */
void
SetWMHints(display, window, initial_state)
Display *display;
Window window;
int initial_state;
{
	XWMHints wm_hints;

	/* which flags are filled in */
	wm_hints.flags = InputHint | StateHint;

	/* start as icon or window */
	wm_hints.initial_state = initial_state;

	/* we want input */
	wm_hints.input = True;

	/* set values */
	XSetWMHints(display, window, &wm_hints);

	/* all done */
	return;
}
