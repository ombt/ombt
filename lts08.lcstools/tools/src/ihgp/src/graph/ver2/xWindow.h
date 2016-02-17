#ifndef __XWINDOW_H
#define __XWINDOW_H
/* class for an X-window */

/* required headers */
#include <sysent.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"
#include "xWindowDisplay.h"

/* X-window headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* local definitions */
#define BORDER_WIDTH 2

/* define class */
class XWindow;

/* ascii window class definition */
class XWindow: public GenericWindow {
private:
	/* don't allow these */
	XWindow() { }
	int init() { return(NOTOK); }

public:
	/* constructor and destructor */
	XWindow(char *, XWindowDisplay *, int, int, int, int, 
		char * = "fixed");
	XWindow(char *, XWindowDisplay *, int, int, int, int, 
		unsigned long, unsigned long, char * = "fixed");
	XWindow(const XWindow &);
	~XWindow();

	/* initialization functions */
	int init(char *, XWindowDisplay *, int, int, int, int, 
		char *);
	int init(char *, XWindowDisplay *, int, int, int, int, 
		unsigned long, unsigned long, char *);
	int init(const XWindow &);
	int deleteAll();

	/* assignment operator */
	XWindow &operator=(const XWindow &);

	/* cursor location */
	int putCursor(int, int);
	int getCursor(int &, int &);

	/* draw points */
	int putPoint();
	int putPoint(unsigned long);
	int putPoint(int, int);
	int putPoint(int, int, unsigned long);

	/* draw characters */
	int putCharacter(int);
	int putCharacter(int, unsigned long);
	int putCharacter(int, int, int);
	int putCharacter(int, int, int, unsigned long);

	/* write strings */
	int putString(char *, Direction = Direction0);
	int putString(int, int, char *, Direction = Direction0);
	int putString(char *, unsigned long, Direction = Direction0);
	int putString(int, int, char *, unsigned long, Direction = Direction0);
	int stringHeight(char *);
	int stringWidth(char *);
	int setFont(char *);

	/* get a character or string */
	int getCharacter(int &);
	int getString(int, char *);

	/* set/get foreground and background colors */
	int setForegroundColor(unsigned long);
	unsigned long getForegroundColor();
	int setBackgroundColor(unsigned long);
	unsigned long getBackgroundColor();
	unsigned long getColorForPoint(int, int);

	/* clear part or entire window */
	int clearWindow();
	int clearWindow(int, int, int, int);

	/* draw lines */
	int drawLine(int, int);
	int drawLine(int, int, int, int);
	int drawLine(int, int, unsigned long);
	int drawLine(int, int, int, int, unsigned long);

	/* draw or fill arcs */
	int drawArc(int, int, int, int, int, int);
	int drawArc(int, int, int, int, int, int, unsigned long);
	int fillArc(int, int, int, int, int, int, unsigned long, unsigned long);

	/* draw or fill boxes */
	int drawBox(int, int, int, int);
	int drawBox(int, int, int, int, unsigned long);
	int fillBox(int, int, int, int, unsigned long, unsigned long);

	/* refresh all or part of window */
	int display();
	int refresh() { return(display()); }
	int refresh(int, int, int, int) { return(display()); }

	/* is point in window */
	inline int in_window(int x, int y) {
		return((0 <= x && x <= sizeHints.width) &&
		       (0 <= y && y <= sizeHints.height));
	};

private:
	/* do not allow the following */
	int fillRegion(unsigned long, unsigned long);
	int fillRegion(int, int, unsigned long, unsigned long);

protected:
	/* internal data */
	int mapped;
	unsigned long eventMask;
	XWindowDisplay *windowDisplay;
	char *windowName;
	XSetWindowAttributes windowAttributes;
	XSizeHints sizeHints;
	XWMHints windowManagerHints;
	Window window;
	XGCValues graphicsContextValues;
	GC graphicsContext;
	Cursor windowCursor;
	char *fontName;
	XFontStruct *font;
};

#endif
