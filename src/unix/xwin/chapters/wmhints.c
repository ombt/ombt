// headers
#include "myxlib.h"

// set window manager hints. the flag initial state indicates
// whether we want the window to be iconized or not. the 
// possible values are: WithDrawnState (0), NormalState(1), 
// and IconicState(3).
void
SetWMHints(Display *display, Window window, int initial_state)
{
	XWMHints wm_hints;

	// which flags are filled in
	wm_hints.flags = InputHint | StateHint;

	// start as icon or window
	wm_hints.initial_state = initial_state;

	// we want input
	wm_hints.input = True;

	// set values
	XSetWMHints(display, window, &wm_hints);

	// all done */
	return;
}
