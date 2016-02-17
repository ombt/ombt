#ifndef __XWINDOW_DISPLAY_H
#define __XWINDOW_DISPLAY_H
/* class for an X-window display */

/* required headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* X-window headers */
#include <X11/Xlib.h>

/* define class */
class XWindow;
class XWindowDisplay;

/* ascii window class definition */
class XWindowDisplay {
	friend class XWindow;

public:
	/* constructor and destructor */
	XWindowDisplay(char *, int = 0);
	~XWindowDisplay();

	/* initialization functions */
	int init(char *, int);
	int deleteAll();

	/* turn on/off X-window synchronization for debugging */
	int xSyncOn();
	int xSyncOff();

	/* miscellaneous */
	inline int getStatus() { return(status); }

private:
	/* don't allow these */
	XWindowDisplay();
	XWindowDisplay(const XWindowDisplay &);
	XWindowDisplay &operator=(const XWindowDisplay &);

protected:
	/* internal data */
	int status;
	char *displayName;
	Display *display;
	int screen;
	Window rootWindow;
};

#endif
