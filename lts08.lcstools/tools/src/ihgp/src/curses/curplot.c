/*
 * this file contains basic routines for character plots
 * using CURSES, and gray levels.
 *
 * this file contains functions for writing to a window, no
 * functions are provided for reading from window. therefore,
 * when input is required, the curses input routines must be used.
 *
 * CAVEAT: the curplot initialization function, cur_initscr(), must
 * always be used, don't use initscr().
 */

#include <curses.h>	/* already includes stdio.h */
#include <math.h>
#include <malloc.h>
#include <signal.h>
#include <varargs.h>
#include <values.h>

/*
 * some local macros.
 */
#define CHECKBITS(flag, bitpat) (((flag) & (bitpat)) == (bitpat))
#define SETBITS(flag, bitpat) (flag) = ((flag) | (bitpat))
#define CLRBITS(flag, bitpat) (flag) = ((flag) & ~(bitpat))

/*
 * local values.
 */
#define MAXLEVEL 11L
#define MAXCOLOR MAXLEVEL - 1L
#define NEWLINE ((unsigned char) '\n')

/*
 * window structure for plotting package, and other data types.
 */
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
struct cur_window
{
    WINDOW *cwin;
    UCHAR ascii2gray[128];
    UCHAR palette[MAXLEVEL];
    UCHAR **gray;
    UCHAR *spare;
    long px;
    long py;
    long maxpx;
    long maxpy;
    long flags;
};
typedef struct cur_window CUR_WINDOW;

/*
 * values for selecting attributes.
 */
#define CUR_NONE 	0L
#define CUR_NL		1L			/* screen */
#define CUR_CBREAK	2L*CUR_NL
#define CUR_ECHO	2L*CUR_CBREAK
#define CUR_RAW		2L*CUR_ECHO
#define CUR_STANDOUT	2L*CUR_RAW
#define CUR_REVERSE	2L*CUR_STANDOUT		/* video */
#define CUR_BOLD	2L*CUR_REVERSE
#define CUR_DIM		2L*CUR_BOLD
#define CUR_BLINK	2L*CUR_DIM
#define CUR_UNDERLINE	2L*CUR_BLINK
#define CUR_CLEAROK	2L*CUR_UNDERLINE
#define CUR_IDLOK	2L*CUR_CLEAROK		/* CURSES options */
#define CUR_KEYPAD	2L*CUR_IDLOK
#define CUR_LEAVEOK	2L*CUR_KEYPAD
#define CUR_META	2L*CUR_LEAVEOK
#define CUR_DELAY	2L*CUR_META
#define CUR_INTRFLUSH	2L*CUR_DELAY
#define CUR_SCROLLOK	2L*CUR_INTRFLUSH
#define CUR_REFRESH	2L*CUR_SCROLLOK		/* curplot options */
#define CUR_TOUCHW	2L*CUR_REFRESH

/*
 * other values.
 */
#define LINE 		1L
#define POINT 		2L*LINE
#define OUTSIDE 	2L*POINT
#define ASCII		2L*OUTSIDE
#define GRAY		2L*ASCII
#define OWASCII		2L*GRAY
#define ASCIIBND	2L*OWASCII
#ifndef _NFILE
#define _NFILE 20
#endif

/*
 * signal traps, resets terminal on way out.
 */
void 
cur_reset(sig)
int sig;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_reset\n");
#endif
#ifdef ERRMSG
    fprintf(stderr,"signal %d received, exit.\n", sig);
#endif
    endwin();
    exit(0);
}

/*
 * initialize physical screen.
 */
void
cur_initscr()
{
    void cur_reset();

#ifdef ENTRY
    fprintf(stdout,"entry cur_initscr\n");
#endif
    initscr();
    signal(SIGINT, cur_reset);
    signal(SIGHUP, cur_reset);
    signal(SIGQUIT, cur_reset);
}

/*
 * exit curses.
 */
void
cur_exitscr()
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_exitscr\n");
#endif
    endwin();
    signal(SIGINT, SIG_DFL);
    signal(SIGHUP, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

/*
 * get a new window.
 */
long
cur_getwindow(window, physx0, physy0, nx, ny)
CUR_WINDOW **window;
long physx0, physy0, nx, ny;
{
    /*
    char *calloc();
     */
    int x, y;

#ifdef ENTRY
    fprintf(stdout,"entry cur_getwindow\n");
#endif
    /*
     * get memory for window structure.
     */
    *window = (CUR_WINDOW *) calloc(1, sizeof(CUR_WINDOW));
    if (window == NULL)
    {
	/* 
	 * could not allocate memory.
	 */
	return (0);
    }
    /*
     * allocate memory for y-axis.
     */
    (*window)->gray = (UCHAR **) calloc(ny, sizeof(UCHAR *));
    if ((*window)->gray == NULL)
    {
	/*
	 * error, release all memory previously allocated.
	 */
	free((char *) *window);
	return (0);
    }
    /*
     * allocate memory for each x-axis row.
     */
    for (y = 0; y < ny; y++)
    {
	(*window)->gray[y] = (UCHAR *) calloc(nx, sizeof(UCHAR));
	if ((*window)->gray[y] == NULL)
	{
	    /*
	     * error, release all memory previously allocated.
	     */
	    for (x = 0; x < y; x++)
	    {
		free((char *) (*window)->gray[x]);
	    }
	    free((char *) (*window)->gray);
	    free((char *) *window);
	    return (0);
	}
    }
    /*
     * get spare buffer for formatting input/output.
     */
    (*window)->spare = (UCHAR *) calloc(2*nx, sizeof(UCHAR));
    if ((*window)->spare == NULL)
    {
	/*
	 * error, release all memory previously allocated.
	 */
	for (x = 0; x < ny; x++)
	{
	    free((char *) (*window)->gray[x]);
	}
	free((char *) (*window)->gray);
	free((char *) *window);
	return (0);
    }

    /*
     * clear array ascii2gray, used for converting picture to
     * gray levels.
     */
    for (x = 0; x < 128; x++)
    {
	(*window)->ascii2gray[x] = (UCHAR) 0;
    }
    /*
     * default "colors" for palette.
     */
    (*window)->palette[0] = ' ';	/* background */
    (*window)->palette[1] = '*';	/* intensity 1 */
    for (x = 2; x < MAXLEVEL; x++)
    {
	(*window)->palette[x] = ' ';	/* intensity x */
    }
    /*
     * maximum x and y value for this window.
     */
    (*window)->maxpx = nx - 1;
    (*window)->maxpy = ny - 1;
    /*
     * call curses function to create window at which the 
     * final picture appears.
     */
    (*window)->cwin = newwin((int) ny, (int) nx, 
		          (int) physy0, (int) physx0);
    if ((*window)->cwin == NULL)
    {
	/*
	 * error, release all memory previously allocated.
	 */
	for (y = 0; y < ny; y++)
	{
	    free((char *) (*window)->gray[y]);
	}
	free((char *) (*window)->gray);
	free((char *) (*window)->spare);
	free((char *) *window);
	return (0);
    }
    /*
     * set cursor at (0,0).
     */
    (*window)->px = 0;
    (*window)->py = 0;
    (void) wmove((*window)->cwin, (int) 0, (int) 0);
    return (1);
}

/*
 * delete a window.
 */
long
cur_delwindow(window)
CUR_WINDOW *window;
{
    int y;

#ifdef ENTRY
    fprintf(stdout,"entry cur_delwindow\n");
#endif
    /*
     * release x-axis row memory.
     */
    for (y = 0; y < window->maxpy; y++)
    {
	free((char *) window->gray[y]);
    }
    /*
     * release y-axis memory.
     */
    free((char *) window->gray);
    /*
     * release spare buffer memory.
     */
    free((char *) window->spare);
    /*
     * release CURSES window structure.
     *
    delwin(window->cwin);
    /*
     * release CUR_WINDOW structure.
     */
    free((char *) window);
    return (1);
}

/*
 * put window cursor at a given point.
 */
long
cur_setcursor(window, px, py)
CUR_WINDOW *window;
long px, py;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setcursor\n");
#endif
    /*
     * is pixel out of range ?
     */
    if (px < 0 || px > window->maxpx)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_setcursor: x out of range.\n");
#endif
	return (0);
    }
    if (py < 0 || py > window->maxpy)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_setcursor: y out of range.\n");
#endif
	return (0);
    }
    /*
     * move cursor.
     */
    window->px = px;
    window->py = py;
    if (wmove(window->cwin, (int) py, (int) px) != OK) return (0);
    /*
     * refresh window, and screen.
     */
    if (CHECKBITS(window->flags, CUR_TOUCHW)) (void) touchwin(window->cwin);
    if (CHECKBITS(window->flags, CUR_REFRESH)) (void) wrefresh(window->cwin);
    return (1);
}

/*
 * get cursor position.
 */
long
cur_getcursor(window, px, py)
CUR_WINDOW *window;
long *px, *py;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_getcursor\n");
#endif
    /*
     * get cursor position.
     */
    *px = window->px;
    *py = window->py;
    return (1);
}

/*
 * put point at present cursor position.
 */
long
cur_putpoint(window, level)
CUR_WINDOW *window;
long level;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_putpoint\n");
#endif
    /*
     * check gray level range.
     */
    if (level < 0 || level > MAXCOLOR)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_putpoint: gray level out of range.\n");
#endif
	return (0);
    }
    /*
     * store gray level.
     */
#ifdef DEBUG
    fprintf(stdout,"cur_putpointp, (px,py) = (%d,%d)\n", 
	    window->px, window->py);
#endif
    window->gray[window->py][window->px] = level;

    (void) wmove(window->cwin, (int) window->py, (int) window->px);
    (void) waddch(window->cwin, window->palette[level]);
    (void) wmove(window->cwin, (int) window->py, (int) window->px);
    /*
     * refresh screen.
     */
    if (CHECKBITS(window->flags, CUR_TOUCHW)) (void) touchwin(window->cwin);
    if (CHECKBITS(window->flags, CUR_REFRESH)) (void) wrefresh(window->cwin);
    return (1);
}

/*
 * put point at given cursor position.
 */
long
cur_putpointp(window, px, py, level)
CUR_WINDOW *window;
long px, py, level;
{
    long cur_setcursor(), cur_putpoint();

#ifdef ENTRY
    fprintf(stdout,"entry cur_putpointp\n");
#endif
    if (cur_setcursor(window, px, py) == 0) return (0);
    if (cur_putpoint(window, level) == 0) return (0);
    return (1);
}

/*
 * put symbol at present cursor position.
 */
long
cur_putsym(window, sym)
CUR_WINDOW *window;
char *sym;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_putsym\n");
#endif
    /*
     * write symbol.
     */
    window->gray[window->py][window->px] = (*sym) | (UCHAR) 0200;

    (void) wmove(window->cwin, (int) window->py, (int) window->px);
    (void) waddch(window->cwin, *sym);
    (void) wmove(window->cwin, (int) window->py, (int) window->px);
    /*
     * refresh screen.
     */
    if (CHECKBITS(window->flags, CUR_TOUCHW)) (void) touchwin(window->cwin);
    if (CHECKBITS(window->flags, CUR_REFRESH)) (void) wrefresh(window->cwin);
    return (1);
}

/*
 * put symbol at given cursor position.
 */
long
cur_putsymp(window, px, py, sym)
CUR_WINDOW *window;
long px, py;
char *sym;
{
    long cur_setcursor(), cur_putsym();

#ifdef ENTRY
    fprintf(stdout,"entry cur_putsymp\n");
#endif
    if (cur_setcursor(window, px, py) == 0) return (0);
    if (cur_putsym(window, sym) == 0) return (0);
    return (1);
}

/*
 * write string of characters, assume null termination.
 * flag determines direction the characters are written.
 */
long
cur_putstring(window, string, flag)
CUR_WINDOW *window;
char string[];
long flag;	/* flag   delx   dely  degrees from x-axis */
		/*                     clockwise.
		/*-----------------------------------------*/
		/*  0      0      -1          270          */
		/*  1      1      -1          315          */
		/*  2      1       0            0          */
		/*  3      1       1           45          */
		/*  4      0       1           90          */
		/*  5     -1       1          135          */
		/*  6     -1       0          180          */
		/*  7     -1      -1          225          */
{
    long cur_putsymp();
    long i, x, y, delx, dely;

#ifdef ENTRY
    fprintf(stdout,"entry cur_putstring\n");
#endif
    /*
     * get step size for direction.
     */
    switch (flag)
    {
    case 0:
	delx = 0;
	dely = -1;
	break;
    case 1:
	delx = 1;
	dely = -1;
	break;
    case 2:
	delx = 1;
	dely = 0;
	break;
    case 3:
	delx = 1;
	dely = 1;
	break;
    case 4:
	delx = 0;
	dely = 1;
	break;
    case 5:
	delx = -1;
	dely = 1;
	break;
    case 6:
	delx = -1;
	dely = 0;
	break;
    case 7:
	delx = -1;
	dely = -1;
	break;
    default:
#ifdef ERRMSG
	fprintf(stderr,"cur_putstring: direction out of range.\n");
#endif
	return (0);
    }
    /*
     * print string.
     */
    i = 0;
    x = window->px;
    y = window->py;
    while (((0 <= x) && (x <= window->maxpx)) &&
	   ((0 <= y) && (y <= window->maxpy)) &&
	   (string[i] != '\0'))
    {
 	if (cur_putsymp(window, x, y, &string[i]) == 0) return(0);
	x += delx;
	y += dely;
	i++;
    }
    return (1);
}

/*
 * write string of characters at given position, assume character
 * string is null terminated. flag determines direction.
 */
long
cur_putstringp(window, string, px, py, flag)
CUR_WINDOW *window;
char string[];
long px, py;
long flag;
{
    long cur_setcursor(), cur_putstring();

#ifdef ENTRY
    fprintf(stdout,"entry cur_putstringp\n");
#endif
    if (cur_setcursor(window,px,py) == 0) return (0);
    if (cur_putstring(window,string,flag) == 0) return (0);
    return (1);
}

/*
 * set background level.
 */
long
cur_setback(window, color)
CUR_WINDOW *window;
char *color;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setback\n");
#endif
    window->palette[0] = *color;
    window->ascii2gray[(USHORT) *color] = 0;
    return (1);
}

/*
 * get background level.
 */
long
cur_getback(window, color)
CUR_WINDOW *window;
char *color;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_getback\n");
#endif
    *color = window->palette[0];
    return (1);
}

/*
 * set foreground color.
 */
long
cur_setfore(window, color, level)
CUR_WINDOW *window;
char *color;
long level;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setfore\n");
#endif
    if (level < 0 || level > MAXCOLOR)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_setfore: gray level out of range.\n");
#endif
	return (0);
    }
    window->palette[level] = *color;
    window->ascii2gray[(USHORT) *color] = level;
    return (1);
}

/*
 * get foreground color.
 */
long
cur_getfore(window, color, level)
CUR_WINDOW *window;
char *color;
long level;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_getfore\n");
#endif
    if (level < 0 || level > MAXCOLOR)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_getfore: gray level out of range.\n");
#endif
	return (0);
    }
    *color = window->palette[level];
    return (1);
}

/*
 * erase window, use background color.
 */
long
cur_erase(window)
CUR_WINDOW *window;
{
    long x, y;

#ifdef ENTRY
    fprintf(stdout,"entry cur_erase\n");
#endif
    for (y = 0; y <= window->maxpy; y++)
    {
	for (x = 0; x <= window->maxpx; x++)
	{
	    (void) wmove(window->cwin, (int) y, (int) x);
	    (void) waddch(window->cwin, window->palette[0]);
	    window->gray[y][x] = 0;
 	}
    }
    (void) wmove(window->cwin, (int) window->py, (int) window->px);
    if (CHECKBITS(window->flags, CUR_TOUCHW)) (void) touchwin(window->cwin);
    if (CHECKBITS(window->flags, CUR_REFRESH)) (void) wrefresh(window->cwin);
    return (1);
}

/*
 * hard erase, just blank the entire window.
 */
long
cur_harderase(window)
CUR_WINDOW *window;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_harderase\n");
#endif
    werase(window->cwin);
    return (1);
}

/*
 * paint part (window) of screen array using given gray level.
 */
long
cur_paintw(window, px0, py0, px1, py1, level)
CUR_WINDOW *window;
long px0,py0;
long px1,py1;
long level;
{
    long y, x;
    long cur_putpointp();

#ifdef ENTRY
    fprintf(stdout,"entry cur_paintw\n");
#endif
    if (level < 0 || level > MAXCOLOR)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_paintw: gray level out of range.\n");
#endif
	return (0);
    }
    if (((px0 <= px1) && (0 <= px0) && (px1 <= window->maxpx)) &&
        ((py0 <= py1) && (0 <= py0) && (py1 <= window->maxpy)))
    {
	for (y = py0; y <= py1; y++)
	{
	    for (x = px0; x <= px1; x++)
	    {
		if (! cur_putpointp(window, x, y, level)) return (0);
	    }
	}
    }
    else
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_paintw: window coordinates are wrong.\n");
#endif
	return(0);
    }
    return (1);
}

/*
 * function to plot an array of (x,y) points or an arc.
 */
long
cur_putarc(window,px,py,np,level,flag)
CUR_WINDOW *window;	/* window structure */
long px[];		/* x-coordinates */
long py[];		/* y-coordinates */
long np;		/* number of points */
long level;		/* gray level */
long flag;		/* = LINE, connect the dots */
			/* = POINT, plot points */
{
    long i;
    long cur_setcursor(),cur_putline(),cur_putpointp();

#ifdef ENTRY
    fprintf(stdout,"entry cur_putarc\n");
#endif
    if ((flag & LINE) == LINE)
    {
	if (cur_setcursor(window,px[0],py[0]) == 0) return (0);
	for (i=1; i<np; i++)
	{
	    if (cur_putline(window,px[i],py[i],level) == 0) return (0);
	}
    }
    else if ((flag & POINT) == POINT)
    {
	for (i=0; i<np; i++)
	{
	    if (cur_putpointp(window,px[i],py[i],level) == 0) return (0);
	}
    }
    return (1);
}

/*
 * write a line from current cursor position to new position using
 * the given intensity (gray level). use Bresenham's integer
 * algorithm.
 */
long
cur_putline(window,px,py,level)
CUR_WINDOW *window;
long px, py, level;
{
    long t, distance, xerr, yerr, delx, dely, incx, incy;
    long x, y;
    long cur_putpointp(), cur_setcursor();

#define IABS(t) (((t) > 0) ? (t) : (0 - t))

#ifdef ENTRY
    fprintf(stdout,"entry cur_putline\n");
#endif
    if (px < 0 || px > window->maxpx)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_putline: x out of range.\n");
#endif
	return (0);
    }
    if (py < 0 || py > window->maxpy)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_putline: y out of range.\n");
#endif
	return (0);
    }
    if (level < 0 || level > MAXCOLOR)
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_putline: gray level out of range.\n");
#endif
 	return (0);
    }
    (void) cur_setcursor(window, window->px, window->py);
    if ((window->px == px) && (window->py == py))
    {
	return (cur_putpoint(window,level));
    }
    xerr = 0;
    yerr = 0;
    delx = px - window->px;
    dely = py - window->py;
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
    x = window->px;
    y = window->py;
    for (t=0; t <= distance+1; t++)
    {
#ifdef DEBUG
	fprintf(stdout,"cur_putline, (x,y) = (%d,%d)\n", x, y);
#endif
	(void) cur_putpointp(window, x, y, level);
#ifdef DEBUG
	fprintf(stdout,"cur_putline, (px,py) = (%d,%d)\n", 
	  	window->px, window->py);
#endif
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
    (void) cur_setcursor(window, x, y);
    return (1);
}

/*
 * write a line between 2 points.
 */
long
cur_putlinep(window,px0,py0,px1,py1,level)
CUR_WINDOW *window;
long px0,py0,px1,py1;
long level;
{
    long cur_setcursor(),cur_putline();

#ifdef ENTRY
    fprintf(stdout,"entry cur_putlinep\n");
#endif
    if (cur_setcursor(window,px0,py0) == 0) return (0);
    if (cur_putline(window,px1,py1,level) == 0) return (0);
    return (1);
}

/*
 * function returns level, color, or ascii for given point.
 */
long
cur_pointcolor(window,px,py,level,ascii)
CUR_WINDOW *window;
long px,py;
long *level;
char *ascii;	/* if gray level, return color */
		/* if ascii, return ascii character */
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_pointcolor\n");
#endif
    if ((px < 0 || px > window->maxpx) ||
        (py < 0 || py > window->maxpy))
    {
	return (OUTSIDE);
    }
    if ((window->gray[py][px] & 0200) == 0200)
    {
	*ascii = window->gray[py][px];
	return (ASCII);		/* ascii character */
    }
    *level = window->gray[py][px];
    *ascii = window->palette[*level];
    return (GRAY);		/* gray level */
}

/*
 * this function fills in contours.
 */
void
cur_fill(window,x,y,level,contour,options)
CUR_WINDOW *window;
long x,y;
long level,contour;
long options;	/* = OWASCII, overwrite ASCII lettering */
		/* = ASCIIBND, ASCII lettering is boundary, also */
{
    char ascii;
    long xy_color,status;
    long cur_pointcolor();
    void cur_fill();

#ifdef ENTRY
    fprintf(stdout,"entry cur_fill\n");
#endif
    status = cur_pointcolor(window,x,y,&xy_color,&ascii);
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
	    (void) cur_putpointp(window,x,y,level);
	    cur_fill(window,(x + 1),y,level,contour,options);
	    cur_fill(window,(x - 1),y,level,contour,options);
	    cur_fill(window,x,(y + 1),level,contour,options);
	    cur_fill(window,x,(y - 1),level,contour,options);
	    break;
	}
	break;
    case GRAY:
	if ((xy_color != contour) && (xy_color != level))
	{
	    (void) cur_putpointp(window,x,y,level);
	    cur_fill(window,(x + 1),y,level,contour,options);
	    cur_fill(window,(x - 1),y,level,contour,options);
	    cur_fill(window,x,(y + 1),level,contour,options);
	    cur_fill(window,x,(y - 1),level,contour,options);
	}
	break;
    }
    return;
}

/*
 * draw an arc for the given center, radius, and arc length.
 */
long
cur_arc(window, level, radius, arc, cx, cy, alpha)
CUR_WINDOW *window;
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

#ifdef ENTRY
    fprintf(stdout,"entry cur_arc\n");
#endif
    cos_alpha = cos(2.0*M_PI*alpha/360.0);
    sin_alpha = sin(2.0*M_PI*alpha/360.0);

    amax = 2.0*M_PI*arc/360.0;
    dela = amax/((double) (2*(window->maxpx + window->maxpy)));

    oldpx = window->px;	/* save center of circle */
    oldpy = window->py;

    for ( i = 0; i <= 2*(window->maxpx + window->maxpy); i++)
    {
	ri = (double) i;
	px = xp2x(radius*cos(ri*dela), radius*sin(ri*dela));
	py = yp2y(radius*cos(ri*dela), radius*sin(ri*dela));
	if (! cur_putpointp(window, px, py, level)) return (0);
    }

    (void) cur_setcursor(window, oldpx, oldpy);

    return (1);
}

/*
 * set terminal attributes.
 */
long
cur_setterm(option, setflag)
long option;
long setflag;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setterm\n");
#endif
    /*
     * buffer input or not.
     */
    if (CHECKBITS(option, CUR_CBREAK))
    {
	if (CHECKBITS(setflag, CUR_CBREAK))
	{
	    cbreak();		/* no input buffer */
	}
	else
	{
	    nocbreak();		/* buffer input */
  	}
    }
    /*
     * echo input immediatedly or not.
     */
    if (CHECKBITS(option, CUR_ECHO))
    {
	if (CHECKBITS(setflag, CUR_ECHO))
	{
	    echo();		/* echo input */
	}
	else
	{
	    noecho();		/* no echo of input */
  	}
    }
    /*
     * carriage return/new line mapping.
     */
    if (CHECKBITS(option, CUR_NL))
    {
	if (CHECKBITS(setflag, CUR_NL))
	{
	    nl();		/* mapping of  nl and cr */
	}
	else
	{
	    nonl();		/* no mapping of nl and cr */
  	}
    }
    /*
     * translation of input characters, or none.
     */
    if (CHECKBITS(option, CUR_RAW))
    {
	if (CHECKBITS(setflag, CUR_RAW))
	{
	    raw();		/* input translation */
	}
	else
	{
	    noraw();		/* no input translation */
  	}
    }
    return (1);
}

/*
 * set window options.
 */
long
cur_setwoption(window, option, setflag)
CUR_WINDOW *window;
long option;
long setflag;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setwopt\n");
#endif
    if (CHECKBITS(option, CUR_CLEAROK))
    {
	if (CHECKBITS(setflag, CUR_CLEAROK))
	{
	    clearok(window->cwin, TRUE);
	}
	else
	{
	    clearok(window->cwin, FALSE);
  	}
    }
    if (CHECKBITS(option, CUR_IDLOK))
    {
	if (CHECKBITS(setflag, CUR_IDLOK))
	{
	    idlok(window->cwin, TRUE);
	}
	else
	{
	    idlok(window->cwin, FALSE);
  	}
    }
    if (CHECKBITS(option, CUR_KEYPAD))
    {
	if (CHECKBITS(setflag, CUR_KEYPAD))
	{
	    keypad(window->cwin, TRUE);
	}
	else
	{
	    keypad(window->cwin, FALSE);
  	}
    }
    if (CHECKBITS(option, CUR_LEAVEOK))
    {
	if (CHECKBITS(setflag, CUR_LEAVEOK))
	{
	    leaveok(window->cwin, TRUE);
	}
	else
	{
	    leaveok(window->cwin, FALSE);
  	}
    }
    if (CHECKBITS(option, CUR_META))
    {
	if (CHECKBITS(setflag, CUR_META))
	{
	    meta(window->cwin, TRUE);
	}
	else
	{
	    meta(window->cwin, FALSE);
  	}
    }
    if (CHECKBITS(option, CUR_DELAY))
    {
	if (CHECKBITS(setflag, CUR_DELAY))
	{
	    nodelay(window->cwin, FALSE);
	}
	else
	{
	    nodelay(window->cwin, TRUE);
  	}
    }
    if (CHECKBITS(option, CUR_INTRFLUSH))
    {
	if (CHECKBITS(setflag, CUR_INTRFLUSH))
	{
	    intrflush(window->cwin, TRUE);
	}
	else
	{
	    intrflush(window->cwin, FALSE);
  	}
    }
    if (CHECKBITS(option, CUR_SCROLLOK))
    {
	if (CHECKBITS(setflag, CUR_SCROLLOK))
	{
	    scrollok(window->cwin, TRUE);
	}
	else
	{
	    scrollok(window->cwin, FALSE);
  	}
    }
    return (1);
}

/*
 * set other window options.
 */
long
cur_options(window, option, setflag)
CUR_WINDOW *window;
long option;
long setflag;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_options\n");
#endif
    if (CHECKBITS(option, CUR_REFRESH))
    {
	if (CHECKBITS(setflag, CUR_REFRESH))
	{
	    SETBITS(window->flags, CUR_REFRESH);
	}
	else
	{
	    CLRBITS(window->flags, CUR_REFRESH);
  	}
    }
    if (CHECKBITS(option, CUR_TOUCHW))
    {
	if (CHECKBITS(setflag, CUR_TOUCHW))
	{
	    SETBITS(window->flags, CUR_TOUCHW);
	}
	else
	{
	    CLRBITS(window->flags, CUR_TOUCHW);
  	}
    }
    return (1);
}

/*
 * set video attributes for a window.
 */
long
cur_setwvideo(window, option, setflag)
CUR_WINDOW *window;
long option;
long setflag;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setwvideo\n");
#endif
    if (CHECKBITS(option, CUR_STANDOUT))
    {
	if (CHECKBITS(setflag, CUR_STANDOUT))
	{
	    wattron(window->cwin, A_STANDOUT);
	}
	else
	{
	    wattroff(window->cwin, A_STANDOUT);
  	}
    }
    if (CHECKBITS(option, CUR_REVERSE))
    {
	if (CHECKBITS(setflag, CUR_REVERSE))
	{
	    wattron(window->cwin, A_REVERSE);
	}
	else
	{
	    wattroff(window->cwin, A_REVERSE);
  	}
    }
    if (CHECKBITS(option, CUR_BOLD))
    {
	if (CHECKBITS(setflag, CUR_BOLD))
	{
	    wattron(window->cwin, A_BOLD);
	}
	else
	{
	    wattroff(window->cwin, A_BOLD);
  	}
    }
    if (CHECKBITS(option, CUR_DIM))
    {
	if (CHECKBITS(setflag, CUR_DIM))
	{
	    wattron(window->cwin, A_DIM);
	}
	else
	{
	    wattroff(window->cwin, A_DIM);
  	}
    }
    if (CHECKBITS(option, CUR_BLINK))
    {
	if (CHECKBITS(setflag, CUR_BLINK))
	{
	    wattron(window->cwin, A_BLINK);
	}
	else
	{
	    wattroff(window->cwin, A_BLINK);
  	}
    }
    if (CHECKBITS(option, CUR_UNDERLINE))
    {
	if (CHECKBITS(setflag, CUR_UNDERLINE))
	{
	    wattron(window->cwin, A_UNDERLINE);
	}
	else
	{
	    wattroff(window->cwin, A_UNDERLINE);
  	}
    }
    return (1);
}

/*
 * refresh entire screen.
 */
long
cur_write(window, tflag)
CUR_WINDOW *window;
long tflag;
{
    long x, y;
    char c[1];
    USHORT oldattrs;

#ifdef ENTRY
    fprintf(stdout,"entry cur_write\n");
#endif
    oldattrs = (USHORT) window->cwin->_attrs;
    for (y = 0; y <= window->maxpy; y++)
    {
	for (x = 0; x <= window->maxpx; x++)
	{
	    if ((window->gray[y][x] & 0200) == 0200)
	    {
		*c = window->gray[y][x] & 0177;
	    }
	    else
	    {
		*c = window->palette[window->gray[y][x]];
	    }
	    window->cwin->_attrs = 
		(USHORT) window->cwin->_y[y][x] & A_ATTRIBUTES;
	    mvwaddch(window->cwin, (int) y, (int) x, *c);
	}
    }
    if (tflag) touchwin(window->cwin);
    wrefresh(window->cwin);
    window->cwin->_attrs = oldattrs;
    return (1);
}

/*
 * set scroll region.
 */
long
cur_setscrollreg(window, top, bottom)
CUR_WINDOW *window;
long top, bottom;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_setscrollreg\n");
#endif
    if (0 <= bottom && bottom <= top && top <= window->maxpy)
    {
	wsetscrreg(window->cwin, (int) top, (int) bottom);
    }
    else
    {
#ifdef ERRMSG
	fprintf(stderr,"cur_setscrollreg: invalid scroll region.\n");
#endif
	return (0);
    }
    return (1);
}

/*
 * scroll window.
 */
long
cur_scroll(window)
CUR_WINDOW *window;
{
#ifdef ENTRY
    fprintf(stdout,"entry cur_scroll\n");
#endif
    scroll(window->cwin);
    if (CHECKBITS(window->flags, CUR_TOUCHW)) (void) touchwin(window->cwin);
    if (CHECKBITS(window->flags, CUR_REFRESH)) (void) wrefresh(window->cwin);
    return (1);
}

/*
 * draw box around window using gray levels.
 */
long
cur_box(window, v, h)
CUR_WINDOW *window;
long v, h;
{
    long xmax, ymax, xmin, ymin;
    long cur_putlinep(), cur_putline();

#ifdef ENTRY
    fprintf(stdout,"entry cur_box\n");
#endif
    xmin = 0L;
    ymin = 0L;
    xmax = window->maxpx;
    ymax = window->maxpy;

    if (! cur_putlinep(window, xmin, ymin, xmax, ymin, h)) return(0);
    if (! cur_putline(window, xmax, ymax, v)) return(0);
    if (! cur_putline(window, xmin, ymax, h)) return(0);
    if (! cur_putline(window, xmin, ymin, v)) return(0);

    if (CHECKBITS(window->flags, CUR_TOUCHW)) (void) touchwin(window->cwin);
    if (CHECKBITS(window->flags, CUR_REFRESH)) (void) wrefresh(window->cwin);
    return (1);
}

/*
 * formatted output.
 */
long
cur_printw(window, px, py, format, flag, va_alist)
CUR_WINDOW *window;
long px, py, flag;	/* flag controls direction for writing */
char *format;
va_dcl			/* variable number of arguments */
{
    int count;
    FILE siop;
    va_list ap;

    int _doprnt();
    long cur_setcursor(), cur_putstring();

#ifdef ENTRY
    fprintf(stdout,"entry cur_printw\n");
#endif
    /*
     * move cursor, if px and py are legal. this allows
     * a way for using present cursor position.
     */
    if (px >= 0 && py >= 0)
    {
	if (! cur_setcursor(window, px, py)) return (0);
    }
    /*
     * fake file descriptor.
     */
    siop._cnt = MAXINT;
    siop._base = siop._ptr = window->spare;
    siop._flag = _IOWRT;
    siop._file = _NFILE;
    /*
     * assign stack pointer to ap.
     */
    va_start(ap);
    /*
     * format output.
     */
    count = _doprnt(format, ap, &siop);
    va_end(ap);
    /*
     * null terminated string.
     */
    *siop._ptr = '\0';
    /*
     * print formatted string on window.
     */
    if (! cur_putstring(window, window->spare, flag)) return (0);
    return (1);
}

/*
 * write gray level screen to a file.
 */
long
cur_writescr(window, lu)
CUR_WINDOW *window;
FILE *lu;
{
    long x,y;

#ifdef ENTRY
    fprintf(stdout,"entry cur_writescr\n");
#endif
    for (y = 0; y <= window->maxpy; y++)
    {
	for (x = 0; x <= window->maxpx; x++)
	{
	    putc(window->gray[y][x], lu);
	}
	putc('\n', lu);
    }
    return(1);
}

/*
 * read gray level screen from a file.
 */
long
cur_readscr(window, lu)
CUR_WINDOW *window;
FILE *lu;
{
    long x,y;
    UCHAR *pstr, c;

#ifdef ENTRY
    fprintf(stdout,"entry cur_readscr\n");
#endif

    /* 
     * erase screen. 
     */
    (void) cur_erase(window);
    /* 
     * read screen from file 
     */
    for (y = 0; (y <= window->maxpy) && (! feof(lu)); y++)
    {
	pstr = &window->gray[y][0];
	while (((c = (UCHAR) getc(lu)) != '\n') && (! feof(lu)))
	{
	    if (pstr <= (&window->gray[y][0] + window->maxpx))
	    {
		*pstr = c;
		pstr++;
	    }
	}
    }
    return(1);
}

/*
 * change one gray level to another.
 */
long
cur_chglvl(window, oldlevel, newlevel)
CUR_WINDOW *window;
long oldlevel, newlevel;
{
    long x,y;

#ifdef ENTRY
    fprintf(stdout,"entry cur_chglvl\n");
#endif

    if (oldlevel < 0 || oldlevel > MAXCOLOR) return(0);
    if (newlevel < 0 || newlevel > MAXCOLOR) return(0);

    for (y = 0; y <= window->maxpy; y++)
    {
	for (x = 0; x <= window->maxpx; x++)
	{
	    if (window->gray[y][x] == (UCHAR) oldlevel)
	    {
		window->gray[y][x] = (UCHAR) newlevel;
	    }
	}
    }
    return(1);
}
