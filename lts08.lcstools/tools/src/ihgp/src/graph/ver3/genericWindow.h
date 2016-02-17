#ifndef __WINDOW_H
#define __WINDOW_H
/* abstract class for a generic window */

/* required headers */
#include "returns.h"
#include "debug.h"

/* define class */
class GenericWindow;

/* some definitions */
#define UnknownColor -1L
#define DegreesInCircle 360

/* window class definition */
class GenericWindow {
public:
	/* enumeration for directions */
	enum Direction { 
		Direction0 = 0, Direction1 = 1, 
		Direction2 = 2, Direction3 = 3, 
		Direction4 = 4, Direction5 = 5, 
		Direction6 = 6, Direction7  = 7
	};

public:
	/* constructor and destructor */
	GenericWindow() { status = OK; }
	GenericWindow(const GenericWindow &src) { status = src.status; }
	virtual ~GenericWindow() { status = OK; }

	/* initialization functions */
	virtual int init() = 0;
	virtual int deleteAll() = 0;
	GenericWindow &operator=(const GenericWindow &rhs) { 
		status = rhs.status; 
		return(*this); 
	}

	/* cursor location */
	virtual int putCursor(int x, int y) = 0;
	virtual int getCursor(int &x, int &y) = 0;

	/* draw points */
	virtual int putPoint() = 0;
	virtual int putPoint(unsigned long color) = 0;
	virtual int putPoint(int x, int y) = 0;
	virtual int putPoint(int x, int y, unsigned long color) = 0;

	/* draw characters */
	virtual int putCharacter(int symbol) = 0;
	virtual int putCharacter(int symbol, unsigned long color) = 0;
	virtual int putCharacter(int x, int y, int symbol) = 0;
	virtual int putCharacter(int x, int y, int symbol, 
				unsigned long color) = 0;

	/* write strings */
	virtual int putString(char *string, 
				Direction = Direction0) = 0;
	virtual int putString(int x, int y, char *string, 
				Direction = Direction0) = 0;
	virtual int putString(char *string, unsigned long color, 
				Direction = Direction0) = 0;
	virtual int putString(int x, int y, char *string, unsigned long color, 
				Direction = Direction0) = 0;
	virtual int stringHeight(char *string) = 0;
	virtual int stringWidth(char *string) = 0;
	virtual int setFont(char *) = 0;

	/* get a character or string */
	virtual int getCharacter(int &character) = 0;
	virtual int getString(int stringlength, char *string) = 0;

	/* set/get foreground and background colors */
	virtual int setForegroundColor(unsigned long color) = 0;
	virtual unsigned long getForegroundColor() = 0;
	virtual int setBackgroundColor(unsigned long color) = 0;
	virtual unsigned long getBackgroundColor() = 0;
	virtual unsigned long getColorForPoint(int x, int y) = 0;

	/* clear part or entire window */
	virtual int clearWindow() = 0;
	virtual int clearWindow(int x0, int y0, int x1, int y1) = 0;

	/* draw lines */
	virtual int drawLine(int x, int y) = 0;
	virtual int drawLine(int x0, int y0, int x1, int y1) = 0;
	virtual int drawLine(int x, int y, unsigned long color) = 0;
	virtual int drawLine(int x0, int y0, int x1, int y1, 
				unsigned long color) = 0;

	/* fill in a region */
	virtual int fillRegion(unsigned long contourcolor, 
				unsigned long fillcolor) = 0;
	virtual int fillRegion(int fx, int fy, unsigned long contourcolor, 
				unsigned long fillcolor) = 0;

	/* draw or fill arcs */
	virtual int drawArc(int cx, int cy, int rx, int ry, 
				int alpha, int arc) = 0;
	virtual int drawArc(int cx, int cy, int rx, int ry, 
				int alpha, int arc, unsigned long color) = 0;
	virtual int fillArc(int cx, int cy, int rx, int ry, int alpha, int arc, 
			unsigned long contourcolor, unsigned long fillcolor) = 0;

	/* draw or fill boxes */
	virtual int drawBox(int x0, int y0, int x1, int y1) = 0;
	virtual int drawBox(int x0, int y0, int x1, int y1, 
			unsigned long color) = 0;
	virtual int fillBox(int x0, int y0, int x1, int y1,
			unsigned long contourcolor, unsigned long fillcolor) = 0;

	/* refresh all or part of window */
	virtual int refresh() = 0;
	virtual int refresh(int x0, int y0, int x1, int y1) = 0;
	virtual int display() = 0;

	/* check if point is in the window */
	virtual int in_window(int x, int y) = 0;

	/* miscellaneous functions */
	inline getStatus() { return(status); }

protected:
	/* internal data */
	int status;
};

#endif
