#ifndef __ASCIIWINDOW_H
#define __ASCIIWINDOW_H
/* class for an ascii character window */

/* required headers */
#include <sysent.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"

/* define class */
class AsciiWindow;

/* definitions */
#define Blank ' '
#define Period '.'
#define DefaultBackground ((int)Blank)
#define DefaultForeground ((int)Period)

/* ascii window class definition */
class AsciiWindow: public GenericWindow {
public:
	/* constructor and destructor */
	AsciiWindow();
	AsciiWindow(int, int, unsigned long = DefaultForeground, 
		unsigned long = DefaultBackground);
	AsciiWindow(const AsciiWindow &);
	~AsciiWindow();

	/* initialization functions */
	int init();
	int init(int, int, unsigned long, unsigned long);
	int init(const AsciiWindow &);
	int deleteAll();

	/* assignment operator */
	AsciiWindow &operator=(const AsciiWindow &);

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
	inline int stringHeight(char *) { return(1); }
	inline int stringWidth(char *string) { return(strlen(string)); }
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

	/* fill a region */
	int fillRegion(unsigned long, unsigned long);
	int fillRegion(int, int, unsigned long, unsigned long);

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
		return((0 <= x && x < size_x) && (0 <= y && y < size_y));
	}

protected:
	/* internal data */
	unsigned long foreground;
	unsigned long background;
	int size_x;
	int size_y;
	int cursor_x;
	int cursor_y;
	char **window;
};

#endif
