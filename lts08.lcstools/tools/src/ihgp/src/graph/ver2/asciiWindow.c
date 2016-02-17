/* functions for ascii window class */

/* required headers */
#include <sysent.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <errno.h>
#include "asciiWindow.h"

/* constructors and destructor */
AsciiWindow::AsciiWindow(): GenericWindow()
{
	init();
}

AsciiWindow::AsciiWindow(int szx, int szy, unsigned long fg, 
		unsigned long bg): GenericWindow()
{
	init(szx, szy, fg, bg);
}

AsciiWindow::AsciiWindow(const AsciiWindow &src): GenericWindow(src)
{
	deleteAll();
	init(src);
}

AsciiWindow::~AsciiWindow()
{
	deleteAll();
}

/* initialization functions */
int
AsciiWindow::init()
{
	/* set default values */
	foreground = DefaultForeground;
	background = DefaultBackground;
	size_x = 0;
	size_y = 0;
	cursor_x = 0;
	cursor_y = 0;
	window = (char **)0;

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::init(int szx, int szy, unsigned long fg, unsigned long bg)
{
	/* check parameters */
	if (szx <= 0 || szy <= 0)
	{
		status = EINVAL;
		ERROR("invalid window sizes.", status);
		return(NOTOK);
	}

	/* set internal data */
	foreground = fg;
	background = bg;
	size_x = szx;
	size_y = szy;
	cursor_x = 0;
	cursor_y = 0;

	/* create a window and copy data */
	window = new char *[size_y];
	if (window == (char **)0)
	{
		status = ENOMEM;
		ERROR("unable to malloc a window.", status);
		return(NOTOK);
	}
	for (int y = 0; y < size_y; y++)
	{
		window[y] = new char [size_x];
		if (window[y] == (char *)0)
		{
			status = ENOMEM;
			ERROR("unable to malloc a window.", status);
			return(NOTOK);
		}
		memset((void *)window[y], (int)background, size_x);
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::init(const AsciiWindow &src)
{
	/* copy data */
	foreground = src.foreground;
	background = src.background;
	size_x = src.size_x;
	size_y = src.size_y;
	cursor_x = src.cursor_x;
	cursor_y = src.cursor_y;

	/* create new window */
	if (src.window != (char **)0)
	{
		window = new char *[src.size_y];
		if (window == (char **)0)
		{
			status = ENOMEM;
			ERROR("unable to malloc a window.", status);
			return(NOTOK);
		}
		for (int y = 0; y < src.size_y; y++)
		{
			window[y] = new char [src.size_x];
			if (window[y] == (char *)0)
			{
				status = ENOMEM;
				ERROR("unable to malloc a window.", status);
				return(NOTOK);
			}
			memcpy((void *)window[y], (void *)src.window[y], 
				src.size_x);
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::deleteAll()
{
	/* delete window */
	if (window != (char **)0)
	{
		for (int y = 0; y < size_y; y++)
		{
			delete [size_x] window[y];
		}
		delete [size_y] window;
		window = (char **)0;
	}

	/* default internal data */
	size_x = 0;
	size_y = 0;
	cursor_x = 0;
	cursor_y = 0;
	foreground = DefaultForeground;
	background = DefaultBackground;

	/* all done */
	status = OK;
	return(OK);
}

/* assignment operator */
AsciiWindow &
AsciiWindow::operator=(const AsciiWindow &rhs)
{
	if (this == &rhs) return(*this);
	deleteAll();
	GenericWindow::operator=(rhs);
	init(rhs);
	return(*this);
}

/* set or get cursor location */
int 
AsciiWindow::putCursor(int cx, int cy)
{
	/* check parameters */
	if ( ! in_window(cx, cy))
	{
		status = EINVAL;
		ERROR("point outside of window.", status);
		return(NOTOK);
	}

	/* update cursor */
	cursor_x = cx;
	cursor_y = cy;

	/* all done */
	status = OK;
	return(OK);
}

int 
AsciiWindow::getCursor(int &cx, int &cy)
{
	/* return cursor location */
	cx = cursor_x;
	cy = cursor_y;

	/* all done */
	status = OK;
	return(OK);
}

/* draw points in window */
int
AsciiWindow::putPoint(int x, int y, unsigned long color)
{
	/* check parameters */
	if (window == (char **)0)
	{
		status = EINVAL;
		ERROR("window not allocated.", status);
		return(NOTOK);
	}

	/* check if point is in window */
	if (in_window(x, y))
	{
		/* update cursor */
		cursor_x = x;
		cursor_y = y;

		/* write point in window */
		window[y][x] = (char)color;
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::putPoint()
{
	return(putPoint(cursor_x, cursor_y, foreground));
}

int
AsciiWindow::putPoint(unsigned long color)
{
	return(putPoint(cursor_x, cursor_y, color));
}

int
AsciiWindow::putPoint(int x, int y)
{
	return(putPoint(x, y, foreground));
}

/* draw characters in window */
int
AsciiWindow::putCharacter(int x, int y, int symbol, unsigned long)
{
	/* check parameters */
	if (window == (char **)0)
	{
		status = EINVAL;
		ERROR("window not allocated.", status);
		return(NOTOK);
	}

	/* check if point is in window */
	if (in_window(x, y))
	{
		/* update cursor */
		cursor_x = x;
		cursor_y = y;

		/* write character */
		window[y][x] = (char)symbol;
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::putCharacter(int symbol)
{
	return(putCharacter(cursor_x, cursor_y, symbol, foreground));
}

int
AsciiWindow::putCharacter(int symbol, unsigned long color)
{
	return(putCharacter(cursor_x, cursor_y, symbol, color));
}

int
AsciiWindow::putCharacter(int x, int y, int symbol)
{
	return(putCharacter(x, y, symbol, foreground));
}

/* write a string to the window */
int
AsciiWindow::putString(int x, int y, char *string, unsigned long, 
			GenericWindow::Direction dir)
{
	int delx, dely;

	/* check parameters */
	if (window == (char **)0)
	{
		status = EINVAL;
		ERROR("window not allocated.", status);
		return(NOTOK);
	}
	if (string == (char *)0)
	{
		status = EINVAL;
		ERROR("string is a null pointer.", status);
		return(NOTOK);
	}

	/* get direction for writing string */
	switch (dir)
	{
	case GenericWindow::Direction0:
		delx = 1;
		dely = 0;
		break;
	case GenericWindow::Direction1:
		delx = 1;
		dely = -1;
		break;
	case GenericWindow::Direction2:
		delx = 0;
		dely = -1;
		break;
	case GenericWindow::Direction3:
		delx = -1;
		dely = -1;
		break;
	case GenericWindow::Direction4:
		delx = -1;
		dely = 0;
		break;
	case GenericWindow::Direction5:
		delx = -1;
		dely = 1;
		break;
	case GenericWindow::Direction6:
		delx = 0;
		dely = 1;
		break;
	case GenericWindow::Direction7:
		delx = 1;
		dely = 1;
		break;
	default:
		status = EINVAL;
		ERROR("invalid direction.", status);
		return(NOTOK);
	}

	/* write string */
	int cx = x;
	int cy = y;
	char *pstr = string;
	for ( ; *pstr != 0; cx += delx, cy += dely, pstr++)
	{
		/* is point in window */
		if (in_window(cx, cy))
		{
			/* write character */
			putCharacter(cx, cy, *pstr);
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::putString(char *string, GenericWindow::Direction dir)
{
	return(putString(cursor_x, cursor_y, string, foreground, dir));
}

int
AsciiWindow::putString(int x, int y, char *string, GenericWindow::Direction dir)
{
	return(putString(x, y, string, foreground, dir));
}

int
AsciiWindow::putString(char *string, unsigned long color, 
			GenericWindow::Direction dir)
{
	return(putString(cursor_x, cursor_y, string, color, dir));
}

/* define font type */
int
AsciiWindow::setFont(char *)
{
	/* not supported */
	status = EINVAL;
	return(NOTOK);
}

/* get a character or string */
int
AsciiWindow::getCharacter(int &character)
{
	character = fgetc(stdin);
	if (ferror(stdin))
	{
		status = errno;
		return(NOTOK);
	}
	else if (feof(stdin))
		status = EOF;
	else
		status = OK;
	return(OK);
}

int
AsciiWindow::getString(int stringlength, char *string)
{
	if (string == (char *)0)
	{
		status = EINVAL;
		ERROR("null string buffer.", status);
		return(NOTOK);
	}
	fgets(string, stringlength, stdin);
	if (ferror(stdin))
	{
		string[0] = 0;
		status = errno;
		return(NOTOK);
	}
	else if (feof(stdin))
	{
		string[0] = 0;
		status = EOF;
	}
	else
	{
		string[stringlength-1] = 0;
		status = OK;
	}
	return(OK);
}

/* set or get foreground and background colors */
int
AsciiWindow::setForegroundColor(unsigned long fg)
{
	/* set foreground and return */
	foreground = fg;
	status = OK;
	return(OK);
}

unsigned long
AsciiWindow::getForegroundColor()
{
	/* return foreground */
	status = OK;
	return(foreground);
}

int
AsciiWindow::setBackgroundColor(unsigned long bg)
{
	/* set foreground and return */
	background = bg;
	status = OK;
	return(OK);
}

unsigned long
AsciiWindow::getBackgroundColor()
{
	/* return background */
	status = OK;
	return(background);
}

unsigned long
AsciiWindow::getColorForPoint(int x, int y)
{
	if (in_window(x, y))
		return(window[y][x]);
	else
		return((unsigned long)UnknownColor);
}

/* clear part or all of a window */
int
AsciiWindow::clearWindow(int x0, int y0, int x1, int y1)
{
	/* check parameters */
	if (window == (char **)0)
	{
		status = EINVAL;
		ERROR("window not allocated.", status);
		return(NOTOK);
	}
	if ( ! in_window(x0, y0) || ! in_window(x1, y1))
	{
		status = EINVAL;
		ERROR("corner points not in window.", status);
		return(NOTOK);
	}
	if (x0 > x1 || y0 > y1)
	{
		status = EINVAL;
		ERROR("invalid corner points.", status);
		return(NOTOK);
	}

	/* calculate width of box */
	int delx = x1 - x0 + 1;

	/* clear box in window */
	for (int y = y0; y <= y1; y++)
	{
		memset((void *)&window[y][x0], (int)background, delx);
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::clearWindow()
{
	return(clearWindow(0, 0, size_x-1, size_y-1));
}

/* draw a line, use Bresenham's algorithm */
int
AsciiWindow::drawLine(int x0, int y0, int x1, int y1, unsigned long color)
{
	int x, y, t, distance, xerr, yerr, delx, dely, incx, incy;

	/* check parameters */
	if (window == (char **)0)
	{
		status = EINVAL;
		ERROR("window not allocated.", status);
		return(NOTOK);
	}
	if ( ! in_window(x0, y0) || ! in_window(x1, y1))
	{
		status = EINVAL;
		ERROR("end points not in window.", status);
		return(NOTOK);
	}

	/* check if start and end are the same */
	if (x0 == x1 && y0 == y1)
	{
		return(putPoint(x0, y0, color));
	}

	/* bresenham's algorithm */
	xerr = 0;
	yerr = 0;
	delx = x1 - x0;
	dely = y1 - y0;
	if (delx > 0)
		incx = 1;
	else if (delx == 0)
		incx = 0;
	else
		incx = -1;
	if (dely > 0)
		incy = 1;
	else if (dely == 0)
		incy = 0;
	else
		incy = -1;
	delx = _ABS(delx);
	dely = _ABS(dely);
	if (delx > dely)
		distance = delx;
	else
		distance = dely;
	x = x0;
	y = y0;
	for (t = 0; t <= distance+1; t++)
	{
		putPoint(x, y, color);
		if (t < (distance+1))
		{
			xerr += delx;
			yerr += dely;
			if (xerr > distance)
			{
				xerr -= distance;
				x += incx;
			}
			if (yerr > distance)
			{
				yerr -= distance;
				y += incy;
			}
		}
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::drawLine(int x1, int y1)
{
	return(drawLine(cursor_x, cursor_y, x1, y1, foreground));
}

int
AsciiWindow::drawLine(int x0, int y0, int x1, int y1)
{
	return(drawLine(x0, y0, x1, y1, foreground));
}

int
AsciiWindow::drawLine(int x1, int y1, unsigned long color)
{
	return(drawLine(cursor_x, cursor_y, x1, y1, color));
}

/* fill a region */
int
AsciiWindow::fillRegion(int fx, int fy, unsigned long contourcolor, 
			unsigned long fillcolor)
{
	/* check if point is outside of window */
	if ( ! in_window(fx, fy)) return(OUTSIDE);

	/* check if point must be painted */
	if (getColorForPoint(fx, fy) != contourcolor &&
	    getColorForPoint(fx, fy) != fillcolor)
	{
		putPoint(fx, fy, fillcolor);
		fillRegion(fx, fy-1, contourcolor, fillcolor);
		fillRegion(fx, fy+1, contourcolor, fillcolor);
		fillRegion(fx-1, fy, contourcolor, fillcolor);
		fillRegion(fx+1, fy, contourcolor, fillcolor);
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::fillRegion(unsigned long contourcolor, unsigned long fillcolor)
{
	return(fillRegion(cursor_x, cursor_y, contourcolor, fillcolor));
}

/* draw or fill arcs */
int
AsciiWindow::drawArc(int cx, int cy, int rx, int ry, int alpha, int arc, 
			unsigned long color)
{
	/* check parameters */
	if (rx <= 0 || ry <= 0)
	{
		status = EINVAL;
		ERROR("axes are less than or equal to 0.", status);
		return(NOTOK);
	}

	/* draw arc */
	for (int theta = alpha; theta <= (alpha+arc); theta++)
	{
		double radians = 
			2.0*M_PI*double(theta)/double(DegreesInCircle);
		int x = int(cx+rx*cos(radians));
		int y = int(cy+ry*sin(radians));
		putPoint(x, y, color);
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::drawArc(int cx, int cy, int rx, int ry, int alpha, int arc)
{
	return(drawArc(cx, cy, rx, ry, alpha, arc, foreground));
}

int
AsciiWindow::fillArc(int cx, int cy, int rx, int ry, int alpha, int arc, 
			unsigned long contourcolor, unsigned long fillcolor)
{
	/* check parameters */
	if (rx <= 0 || ry <= 0)
	{
		status = EINVAL;
		ERROR("axes are less than or equal to 0.", status);
		return(NOTOK);
	}

	/* check if a full circle is drawn */
	if (arc >= DegreesInCircle)
	{
		/* draw a full circle */
		if (drawArc(cx, cy, rx, ry, alpha, 
			DegreesInCircle, contourcolor) != OK)
		{
			ERROR("drawArc failed.", status);
			return(NOTOK);
		}
	}
	else
	{
		/* draw lines and arc */
		double radians = 
			2.0*M_PI*double(alpha)/double(DegreesInCircle);
		int x1 = int(cx+rx*cos(radians));
		int y1 = int(cy+ry*sin(radians));
		radians = 2.0*M_PI*double(alpha+arc)/double(DegreesInCircle);
		int x2 = int(cx+rx*cos(radians));
		int y2 = int(cy+ry*sin(radians));
		if (drawLine(cx, cy, x1, y2, contourcolor) != OK)
		{
			ERROR("drawLine failed.", status);
			return(NOTOK);
		}
		if (drawLine(cx, cy, x1, y1, contourcolor) != OK)
		{
			ERROR("drawLine failed.", status);
			return(NOTOK);
		}
		if (drawArc(cx, cy, rx, ry, alpha, arc, contourcolor) != OK)
		{
			ERROR("drawArc failed.", status);
			return(NOTOK);
		}
	}

	/* fill in region */
	if (fillRegion(cx, cy, contourcolor, fillcolor) != OK)
	{
		ERROR("fillRegion failed.", status);
		return(NOTOK);
	}

	/* all done */
	status = OK;
	return(OK);
}

/* draw or fill a box */
int
AsciiWindow::drawBox(int x0, int y0, int x1, int y1, unsigned long color)
{
	/* check parameters */
	if (x0 > x1 || y0 > y1)
	{
		status = EINVAL;
		ERROR("invalid corners for box.", status);
		return(NOTOK);
	}

	/* draw lines for box */
	if (drawLine(x0, y0, x1, y0, color) != OK)
	{
		ERROR("drawLine failed.", status);
		return(NOTOK);
	}
	if (drawLine(x1, y0, x1, y1, color) != OK)
	{
		ERROR("drawLine failed.", status);
		return(NOTOK);
	}
	if (drawLine(x1, y1, x0, y1, color) != OK)
	{
		ERROR("drawLine failed.", status);
		return(NOTOK);
	}
	if (drawLine(x0, y1, x0, y0, color) != OK)
	{
		ERROR("drawLine failed.", status);
		return(NOTOK);
	}

	/* all done */
	status = OK;
	return(OK);
}

int
AsciiWindow::drawBox(int x0, int y0, int x1, int y1)
{
	return(drawBox(x0, y0, x1, y1, foreground));
}

int
AsciiWindow::fillBox(int x0, int y0, int x1, int y1, unsigned long contourcolor, 
			unsigned long fillcolor)
{
	/* draw box */
	if (drawBox(x0, y0, x1, y1, contourcolor) != OK)
	{
		ERROR("drawBox failed.", status);
		return(NOTOK);
	}

	/* fill in region */
	if (fillRegion((x0+x1)/2, (y0+y1)/2, contourcolor, fillcolor) != 0)	
	{
		ERROR("fillRegion failed.", status);
		return(NOTOK);
	}

	/* all done */
	status = OK;
	return(OK);
}

/* display screen */
int
AsciiWindow::display()
{
	/* check parameters */
	if (window == (char **)0)
	{
		status = EINVAL;
		ERROR("window not allocated.", status);
		return(NOTOK);
	}

	/* save character at cursor location */
	int saveChar = window[cursor_y][cursor_x];

	/* put cursor in */
	putCharacter(int('C'));

	/* print picture to stdout */
	char *pbuf = new char [size_x+1];
	if (pbuf == (char *)0)
	{
		status = ENOMEM;
		ERROR("unable to malloc a buffer.", status);
		return(NOTOK);
	}
	for (int y = 0; y < size_y; y++)
	{
		memcpy((void *)pbuf, (void *)window[y], size_x);
		pbuf[size_x] = 0;
		fprintf(stdout, "%s\n", pbuf);
	}
	delete pbuf;

	/* restore character */
	putCharacter(saveChar);

	/* all done */
	status = OK;
	return(OK);
}
