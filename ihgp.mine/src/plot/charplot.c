/*
 * this file contains some basic routines for character plots.
 */

#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * maximum number of pixels in screen.
 */
#define MAXX 201L
#define MAXY 201L
long MAXPX = MAXX - 1L;		/* default screen x size */
long MAXPY = MAXY - 1L;		/* default screen y size */
/*
 * Define screen array. The x and y coordinates may appear backwards,
 * however, C stores arrays by rows, not columns. In other words,
 * array elements screen[m][n] and screen[m][n+1] are adjacent bytes,
 * but array elements screen[m][n] and screen[m+1][n] are not adjacent
 * bytes. This array stores intensities, or gray level images.
 * Another array is used to convert the gray level to a "color".
 */
unsigned char screen[MAXY][MAXX];
/*
 * define an aray to convert ASCII to gray level.
 */
static unsigned char ascii2gray[128] = 
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * "color" palette.
 */
#define MAXLEVEL 11L
#define MAXCOLOR MAXLEVEL - 1L
unsigned char palette[MAXLEVEL]= { ' ',		/* background */
			  '*',		/* intensity 1 */
			  ' ',		/* intensity 2 */
			  ' ',		/*    etc.     */
			  ' ',
			  ' ',
			  ' ',
			  ' ',
			  ' ',
			  ' ',
			  ' ' };
/*
 * current position in graph. after each operation, the current
 * cursor position must be set.
 */
long cursorx;
long cursory;
/*
 * flags for horizontal or vertical, etc.
 */
#define VERTICAL 1L
#define HORIZONTAL 2L
#define UP 4L
#define DOWN 8L
#define LEFT 16L
#define RIGHT 32L
#define LINE 64L
#define POINT 128L
#define OUTSIDE 256L
#define ASCII 512L
#define GRAY 1024L
#define OWASCII 2048L
#define ASCIIBND 4096L
/*
 * other stuff.
 */
#define NEWLINE ((unsigned char) '\n')
/*
 * set screen size.
 */
long char_setscr(maxpx,maxpy)

long maxpx, maxpy;
{
    if (maxpx < 0 || maxpx >= MAXX)
    {
	fprintf(stderr,"char_setscr: x size out of range.\n");
	return (0);
    }
    if (maxpy < 0 || maxpy >= MAXY)
    {
	fprintf(stderr,"char_setscr: y size out of range.\n");
	return (0);
    }
    MAXPX = maxpx;
    MAXPY = maxpy;
    return (1);
}

/*
 * get present screen size.
 */
long char_getscr(maxpx,maxpy)

long *maxpx, *maxpy;
{
    *maxpx = MAXPX;
    *maxpy = MAXPY;
    return (1);
}

/*
 * get absolute maximum screen size. the maximum screen size
 * is determined by the size of the matrix, screen.
 */
long char_getmaxscr(maxpx,maxpy)

long *maxpx, *maxpy;
{
    *maxpx = MAXX - 1L;
    *maxpy = MAXY - 1L;
    return (1);
}

/*
 * function to set background color.
 */
long char_setback(color)

char color[];
{
    palette[0] = *color;	/* 0 always background */
    ascii2gray[(unsigned short) *color] = 0;
    return (1);
}

/*
 * function to set foreground color for a gray level.
 */
long char_setfore(color,level)

char color[];
long level;
{
    if (level < 1 || level > MAXCOLOR)
    {
	fprintf(stderr,"char_setfore: gray level out of range.\n");
	return (0);
    }
    palette[level] = *color;
    ascii2gray[(unsigned short) *color] = level;
    return (1);
}

/*
 * function to get background color.
 */
long char_getback(color)

char color[];
{
    *color = palette[0];	/* 0 is background level */
    return (1);
}

/*
 * function to get foreground color for a gray level.
 */
long char_getfore(color,level)

char color[];
long level;
{
    if (level < 1 || level > MAXCOLOR)
    {
	fprintf(stderr,"char_getfore: gray level out of range.\n");
	return (0);
    }
    *color = palette[level];
    return (1);
}

/*
 * erase screen array using background color.
 */
long char_erase()
{
    long y, x;
    for (y=0; y<=MAXPY; y++)
    {
	for (x=0; x<=MAXPX; x++)
	{
	    screen[y][x] = 0;	/* gray level 0 is background */
	}
    }
    /*
     * leave cursor position unchanged.
     */
    return (1);
}

/*
 * paint part (window) of screen array using given gray level.
 */
long char_paintw(px0,py0,px1,py1,level)

long px0,py0;	/* lower LHS corner of window */
long px1,py1;	/* upper RHS corner of window */
long level;
{
    long y, x;
    if (level < 0 || level > MAXCOLOR)
    {
	fprintf(stderr,"char_paintw: gray level out of range.\n");
	return (0);
    }
    if (((px0 <= px1) && (0 <= px0) && (px1 <= MAXPX)) &&
        ((py0 <= py1) && (0 <= py0) && (py1 <= MAXPY)))
    {
	for (y=py0; y<=py1; y++)
	{
	    for (x=px0; x<=px1; x++)
	    {
		screen[y][x] = level;	/* gray level for window */
	    }
	}
    }
    else
    {
	fprintf(stderr,"char_paintw: window coordinates are wrong.\n");
	return(0);
    }
    /*
     * leave current cursor position unchanged.
     */
    return (1);
}

/*
 * set cursor at a point.
 */
long char_setcursor(px, py)

long px, py;
{
    if (px < 0 || px > MAXPX)
    {
	fprintf(stderr,"char_setcursor: x out of range.\n");
	return (0);
    }
    if (py < 0 || py > MAXPY)
    {
	fprintf(stderr,"char_setcursor: y out of range.\n");
	return (0);
    }
    cursorx = px;
    cursory = py;
    return (1);
}

/*
 * get cursor position.
 */
long char_getcursor(px, py)

long *px, *py;
{
    *px = cursorx;
    *py = cursory;
    return (1);
}

/*
 * put symbol at present cursor position.
 */
long char_putsym(sym)

char sym[];
{
    screen[cursory][cursorx] = *sym;
    screen[cursory][cursorx] = 
	screen[cursory][cursorx] | (unsigned char) 0200;
    return (1);
}

/*
 * put symbol at given cursor position.
 */
long char_putsymp(px, py, sym)

long px, py;
char sym[];
{
    long char_setcursor(), char_putsym();
    if (char_setcursor(px, py) == 0) return(0);
    if (char_putsym(sym) == 0) return(0);
    return(1);
}

/*
 * put point at present cursor position.
 */
long char_putpoint(level)

long level;
{
    if (level < 0 || level > MAXCOLOR)
    {
	fprintf(stderr,"char_putpoint: gray level out of range.\n");
	return (0);
    }
    screen[cursory][cursorx] = level;
    return (1);
}

/*
 * put point at given cursor position.
 */
long char_putpointp(px,py,level)

long level;
{
    long char_setcursor(), char_putpoint();
    if (char_setcursor(px, py) == 0) return(0);
    if (char_putpoint(level) == 0) return(0);
    return(1);
}

/*
 * write a string of characters, assume character string
 * is null terminated. flag determines which direction string 
 * is written, up, down, left or right.
 */
long char_putstring(string,flag)

char string[];
long flag;	/* flag   delx   dely  degrees from x-axis */
		/*                     clockwise.
		/*-----------------------------------------*/
		/*  0      0      1           270          */
		/*  1      1      1           315          */
		/*  2      1      0             0          */
		/*  3      1     -1            45          */
		/*  4      0     -1            90          */
		/*  5     -1     -1           135          */
		/*  6     -1      0           180          */
		/*  7     -1      1           225          */
{
    long i, x, y, delx, dely;
    switch (flag)
    {
    case 0:
	delx = 0;
	dely = 1;
	break;
    case 1:
	delx = 1;
	dely = 1;
	break;
    case 2:
	delx = 1;
	dely = 0;
	break;
    case 3:
	delx = 1;
	dely = -1;
	break;
    case 4:
	delx = 0;
	dely = -1;
	break;
    case 5:
	delx = -1;
	dely = -1;
	break;
    case 6:
	delx = -1;
	dely = 0;
	break;
    case 7:
	delx = -1;
	dely = 1;
	break;
    default:
	fprintf(stderr,"char_putstring: direction out of range.\n");
	return (0);
    }
    i = 0;
    x = cursorx;
    y = cursory;
    while (((0 <= x) && (x <= MAXPX)) &&
	   ((0 <= y) && (y <= MAXPY)) &&
	   (string[i] != '\0'))
    {
	screen[y][x] = string[i];
	screen[y][x] = screen[y][x] | (unsigned char) 0200;
	x += delx;
	y += dely;
	i++;
    }
    cursorx = x - delx;		/* restore to last position */
    cursory = y - dely;		/* restore to last position */
    return (1);
}

/*
 * write string of characters at given position, assume character
 * string is null terminated. flag determines direction
 * string is written, up, down, left, right.
 */
long char_putstringp(string,px,py,flag)

char string[];
long px, py;
long flag;
{
    long char_setcursor(), char_putstring();
    if (char_setcursor(px,py) == 0) return (0);
    if (char_putstring(string,flag) == 0) return (0);
    return (1);
}

/*
 * write screen to lu.
 */
long char_writescr(lu, scale)

FILE *lu;
long scale;
{
    long x,y;
    for (y=MAXPY; y>=0; y--)
    {
	for (x=0; x<=MAXPX; x++)
	{
	    if ((screen[y][x] & 0200) == 0200)
	    {
		fprintf(lu,"%c",(screen[y][x] & 0177));
	    }
	    else
	    {
		fprintf(lu,"%c",palette[screen[y][x]]);
	    }
	    if (scale) fprintf(lu," ");
	}
	fprintf(lu,"\n");
    }
    return (1);
}

/*
 * read lu and store in screen. assume all input are gray
 * level characters.
 */
long char_readscr(lu, scale)

FILE *lu;
long scale;
{
    long y;
    unsigned char *pstr, *pstrend, c;
    /*
     * erase screen.
     */
    (void) char_erase();
    /*
     * now read in file.
     */
    for (y = MAXPY; (y >= 0) && (! feof(lu)); y--)
    {
	pstr = &screen[y][0];
	while ((! feof(lu)) && ((c = (unsigned char) getc(lu)) != NEWLINE))
	{
	    if (!(scale && (c == ' ')))
	    {
		if (pstr <= (&screen[y][0]+MAXPX))
		{
		    *pstr = c;
		    pstrend = pstr;
		    pstr++;
		    pstrend++;
		}
	    }
	}
	*pstrend = '\0';
	/*
	 * convert ASCII to gray level.
	 */
	pstr = &screen[y][0];
	for ( ; *pstr != '\0'; pstr++)
	{
	    *pstr = ascii2gray[(unsigned short) *pstr];
	}
    }
    return (1);
}

/*
 * function to plot an array of (x,y) points or an arc.
 */
long char_putarc(px,py,np,level,flag)

long px[];	/* x-coordinates */
long py[];	/* y-coordinates */
long np;	/* number of points */
long level;	/* gray level */
long flag;	/* = LINE, connect the dots */
		/* = POINT, plot points */
{
    long i;
    long char_setcursor(),char_putline(),char_putpointp();
    if ((flag & LINE) == LINE)
    {
	if (char_setcursor(px[0],py[0]) == 0) return (0);
	for (i=1; i<np; i++)
	{
	    if (char_putline(px[i],py[i],level) == 0) return (0);
	}
    }
    else if ((flag & POINT) == POINT)
    {
	for (i=0; i<np; i++)
	{
	    if (char_putpointp(px[i],py[i],level) == 0) return (0);
	}
    }
    return (1);
}

/*
 * write a line from current cursor position to new position using
 * the given intensity (gray level). use Bresenham's integer
 * algorithm.
 */
long char_putline(px,py,level)

long px, py, level;
{
    long t, distance, xerr, yerr, delx, dely, incx, incy;
    long x, y;

#define IABS(t) (((t) > 0) ? (t) : (0 - t))

    if (px < 0 || px > MAXPX)
    {
	fprintf(stderr,"char_putline: x out of range.\n");
	return (0);
    }
    if (py < 0 || py > MAXPY)
    {
	fprintf(stderr,"char_putline: y out of range.\n");
	return (0);
    }
    if (level < 0 || level > MAXCOLOR)
    {
	fprintf(stderr,"char_putline: gray level out of range.\n");
 	return (0);
    }
    if ((cursorx == px) && (cursory == py))
    {
	return (char_putpoint(level));
    }
    xerr = 0;
    yerr = 0;
    delx = px - cursorx;
    dely = py - cursory;
    if (delx > 0) 
    {
	incx = 1;
    }
    else if (delx == 0)
    {
	incx = 0;
    }
    else
    {
	incx = -1;
    }
    if (dely > 0) 
    {
	incy = 1;
    }
    else if (dely == 0)
    {
	incy = 0;
    }
    else
    {
	incy = -1;
    }
    delx = IABS(delx);
    dely = IABS(dely);
    if (delx > dely)
    {
	distance = delx;
    }
    else
    {
	distance = dely;
    }
    x = cursorx;
    y = cursory;
    for (t=0; t <= distance+1; t++)
    {
	screen[y][x] = level;	/* gray level for line */
	if (t < (distance + 1))
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
    cursorx = x;
    cursory = y;
    return (1);
}

/*
 * write a line between 2 points.
 */
long char_putlinep(px0,py0,px1,py1,level)

long px0,py0,px1,py1;
long level;
{
    long char_setcursor(),char_putline();
    if (char_setcursor(px0,py0) == 0) return (0);
    if (char_putline(px1,py1,level) == 0) return (0);
    return (1);
}

/*
 * function returns level, color, or ascii for given point.
 */
long char_pointcolor(px,py,level,ascii)

long px,py;
long *level;
char *ascii;	/* if gray level, return color */
		/* if ascii, return ascii character */
{
    if ((px < 0 || px > MAXPX) ||
        (py < 0 || py > MAXPY))
    {
	return (OUTSIDE);
    }
    if ((screen[py][px] & 0200) == 0200)
    {
	*ascii = screen[py][px];
	return (ASCII);		/* ascii character */
    }
    *level = screen[py][px];
    *ascii = palette[*level];
    return (GRAY);		/* gray level */
}

/*
 * this function fills in contours.
 */
void char_fill(x,y,level,contour,options)

long x,y;
long level,contour;
long options;	/* = OWASCII, overwrite ASCII lettering */
		/* = ASCIIBND, ASCII lettering is boundary, also */
{
    char ascii;
    long xy_color,status;
    long char_pointcolor();

    status = char_pointcolor(x,y,&xy_color,&ascii);
    switch (status)
    {
    case OUTSIDE:
	break;
    case ASCII:
	switch (options)
	{
	case ASCIIBND: 
	    break;
	case OWASCII:
	default:
	    (void) char_putpointp(x,y,level);
	    char_fill((x + 1),y,level,contour,options);
	    char_fill((x - 1),y,level,contour,options);
	    char_fill(x,(y + 1),level,contour,options);
	    char_fill(x,(y - 1),level,contour,options);
	    break;
	}
	break;
    case GRAY:
	if ((xy_color != contour) && (xy_color != level))
	{
	    (void) char_putpointp(x,y,level);
	    char_fill((x + 1),y,level,contour,options);
	    char_fill((x - 1),y,level,contour,options);
	    char_fill(x,(y + 1),level,contour,options);
	    char_fill(x,(y - 1),level,contour,options);
	}
	break;
    }
    return;
}

/*
 * draw an arc for the given center, radius, and arc length.
 */
long char_arc(level, radius, arc, cx, cy, alpha)

long level;	/* gray level for arc */
double radius;	/* radius of circle */
double arc;	/* arc to draw, degrees measured from rotated x-axis */
double cx, cy;	/* center of circle */
double alpha;	/* angle between rotated x-axis and fixed x-axis */
{
    long i, px, py, oldpx, oldpy;
    double ri, amax, dela, cos_alpha, sin_alpha;

#define xp2x(xp, yp) ((xp)*cos_alpha - (yp)*sin_alpha + cx)
#define yp2y(xp, yp) ((xp)*sin_alpha + (yp)*cos_alpha + cy)

    cos_alpha = cos(2.0*M_PI*alpha/360.0);
    sin_alpha = sin(2.0*M_PI*alpha/360.0);

    amax = 2.0*M_PI*arc/360.0;
    dela = amax/((double) (2*(MAXPX + MAXPY)));

    oldpx = cursorx;	/* save center of circle */
    oldpy = cursory;

    for ( i = 0; i <= 2*(MAXPX + MAXPY); i++)
    {
	ri = (double) i;
	px = xp2x(radius*cos(ri*dela), radius*sin(ri*dela));
	py = yp2y(radius*cos(ri*dela), radius*sin(ri*dela));
	if (! char_putpointp(px, py, level)) return (0);
    }

    cursorx = oldpx;	/* restore cursor to center */
    cursory = oldpy;

    return (1);
}
