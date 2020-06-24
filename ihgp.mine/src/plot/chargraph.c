/*
 * this file contains functions for drawing graphs.
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "charplot.h"
#include "charconv.h"

/*
 * options for this package.
 */
#define AUTOXSCALE 1
#define AUTOYSCALE 2
#define CONNECT 4
#define DOT 8
#define USEF 16
#define USEE 32

/*
 * function for plotting an array of points.
 */
long char_putxy(x,y,n,x0,y0,x1,y1,px0,py0,px1,py1,level,options)

double x[];	/* x-coordinates */
double y[];	/* y-coordinates */
long n;		/* number of points */
double x0,y0;	/* minimum x and y values to plot */
double x1,y1;	/* maximum x and y values to plot */
long px0,py0;	/* lower LHS pixel coordinates for graph */
long px1,py1;	/* upper RHS pixel coordinates for graph */
long level;	/* gray level for plot */
long options;	/* = AUTOXSCALE, auto-scale in x-direction */
		/* = AUTOYSCALE, auto-scale in y-direction */
		/* = CONNECT, connect the dots */
		/* = DOT, dot plot */
{
    long char_getscr();

    long maxpx,maxpy;
    long px[20],py[20];
    double mx,my;
    long i,j,flag;

    /*
     * get screen size, and check pixel coordinates.
     */
    if (char_getscr(&maxpx, &maxpy) == 0)
    {
	fprintf(stderr,"char_plotxy: unable to get screen size.\n");
	return (0);
    }
    if (!((px0 < px1) && (0 <= px0) && (px1 <= maxpx)))
    {
	fprintf(stderr,"char_plotxy: x pixels out of range.\n");
	return (0);
    }
    if (!((py0 < py1) && (0 <= py0) && (py1 <= maxpy)))
    {
	fprintf(stderr,"char_plotxy: y pixels out of range.\n");
	return (0);
    }
    /*
     * auto-scale in x or y directions.
     */
    if ((options & AUTOXSCALE) == AUTOXSCALE)
    {
 	x0 = x[0];		/* minimum x */
 	x1 = x[0];		/* maximum x */
	for (i=1; i<n; i++)
	{
	    if (x[i] < x0) x0 = x[i];
	    if (x[i] > x1) x1 = x[i];
	}
    }
    if ((options & AUTOYSCALE) == AUTOYSCALE)
    {
 	y0 = y[0];		/* minimum y */
 	y1 = y[0];		/* maximum y */
	for (i=1; i<n; i++)
	{
	    if (y[i] < y0) y0 = y[i];
	    if (y[i] > y1) y1 = y[i];
	}
    }
    /*
     * plot points in groups of 20.
     */
    if ((options & DOT) == DOT)
    {
	flag = (flag | POINT);
    }
    else if ((options & CONNECT) == CONNECT)
    {
	flag = (flag | LINE);
    }
    mx = ((double) (px1 - px0))/(x1 - x0);
    my = ((double) (py1 - py0))/(y1 - y0);
    j = 0;
    for (i=0; i<n; i++)
    {
	px[j] = mx*(x[i] - x0) + px0;
	py[j] = my*(y[i] - y0) + py0;
	if (j == 19)
	{
	    j++;
	    if (char_putarc(px,py,j,level,flag) == 0)
	    {
		fprintf(stderr,"char_putxy: put arc failed.\n");
	 	return (0);
	    }
	    px[0] = px[19];
	    py[0] = py[19];
	    j=1;
	}
	else
	{
	    j++;
	}
    }
    return (1);
}

/*
 * function to plot a crude graph with minimum and maximum x,y values
 * labeled/
 */
long char_graph(x,y,n,x0,y0,x1,y1,xlabel,ylabel,plot,axes,options)

double x[];	/* x-coordinates */
double y[];	/* y-coordinates */
long n;		/* number of points */
double x0,y0;	/* minimum x and y values to plot. */
double x1,y1;	/* maximum x and y values to plot. */
char xlabel[];	/* x-axis label */
char ylabel[];	/* y-axis label */
long plot;	/* gray level for plot */
long axes;	/* gray level for x,y axes */
long options;	/* = AUTOXSCALE, auto-scale in x-direction */
		/* = AUTOYSCALE, auto-scale in y-direction */
		/* = CONNECT, connect the dots */
		/* = DOT, dot plot */
		/* = USEE, use E format */
		/* = USEF, use F format */
{
    long xoff, yoff, maxpx, maxpy, i;
    struct {
	char label[12];
	long length;
    } axis[4];
    
    long char_putxy(), char_length();
    /*
     * get screen size.
     */
    if (char_getscr(&maxpx, &maxpy) == 0)
    {
	fprintf(stderr,"char_graph: unable to get screen size.\n");
	return (0);
    }
    /*
     * auto-scale in x or y directions.
     */
    if ((options & AUTOXSCALE) == AUTOXSCALE)
    {
 	x0 = x[0];		/* minimum x */
 	x1 = x[0];		/* maximum x */
	for (i=1; i<n; i++)
	{
	    if (x[i] < x0) x0 = x[i];
	    if (x[i] > x1) x1 = x[i];
	}
	options = (options & ~AUTOXSCALE);	/* turn off scaling */
    }
    if ((options & AUTOYSCALE) == AUTOYSCALE)
    {
 	y0 = y[0];		/* minimum y */
 	y1 = y[0];		/* maximum y */
	for (i=1; i<n; i++)
	{
	    if (y[i] < y0) y0 = y[i];
	    if (y[i] > y1) y1 = y[i];
	}
	options = (options & ~AUTOYSCALE);	/* turn off scaling */
    }
    /*
     * get labels.
     */
    if ((options & USEE) == USEE)
    {
	(void) char_d2a_e(x0, 9L, 3L, axis[0].label, &axis[0].length);
	(void) char_d2a_e(x1, 9L, 3L, axis[1].label, &axis[1].length);
	(void) char_d2a_e(y0, 9L, 3L, axis[2].label, &axis[2].length);
	(void) char_d2a_e(y1, 9L, 3L, axis[3].label, &axis[3].length);
	xoff = 11L;
	yoff = 2L;
    }
    else
    {
	(void) char_d2a_f(x0, 9L, 3L, axis[0].label, &axis[0].length);
	(void) char_d2a_f(x1, 9L, 3L, axis[1].label, &axis[1].length);
	(void) char_d2a_f(y0, 9L, 3L, axis[2].label, &axis[2].length);
	(void) char_d2a_f(y1, 9L, 3L, axis[3].label, &axis[3].length);
	xoff = 8L;
	yoff = 2L;
    }
    /*
     * Draw axes.
     */
    if (char_putlinep(xoff, yoff, (maxpx - xoff/2 - 1), yoff, axes) != 1)
    {
	fprintf(stderr,"char_graph: error drawing axes.\n");
	return (0);
    }
    if (char_putline((maxpx - xoff/2 - 1), (maxpy - yoff), axes) != 1)
    {
	fprintf(stderr,"char_graph: error drawing axes.\n");
	return (0);
    }
    if (char_putline(xoff, (maxpy - yoff), axes) != 1)
    {
	fprintf(stderr,"char_graph: error drawing axes.\n");
	return (0);
    }
    if (char_putline(xoff, yoff, axes) != 1)
    {
	fprintf(stderr,"char_graph: error drawing axes.\n");
	return (0);
    }
    /*
     * put labels.
     */
    if (char_putstringp(axis[0].label, 
       (xoff - axis[0].length/2 - 1), (yoff - 1), 2L) != 1)
    {
	fprintf(stderr,"char_graph: error writing labels.\n");
	return (0);
    }
    if (char_putstringp(axis[1].label, 
       (maxpx - xoff/2 - axis[1].length/2 - 1), (yoff - 1), 2L) != 1)
    {
	fprintf(stderr,"char_graph: error writing labels.\n");
	return (0);
    }
    if (char_putstringp(axis[2].label, 
       (xoff - axis[2].length - 1), yoff, 2L) != 1)
    {
	fprintf(stderr,"char_graph: error writing labels.\n");
	return (0);
    }
    if (char_putstringp(axis[3].label, 
        (xoff - axis[3].length - 1), (maxpy - yoff), 2L) != 1)
    {
	fprintf(stderr,"char_graph: error writing labels.\n");
	return (0);
    }
    /*
     * axes labels.
     */
    if (char_putstringp(xlabel, 
	(maxpx + xoff/2 - 1 - char_length(xlabel))/2, 0L, 2L) != 1)
    {
	fprintf(stderr,"char_graph: error writing labels.\n");
	return (0);
    }
    if (char_putstringp(ylabel, 
	(xoff - 2), (maxpy + char_length(ylabel))/2, 4L) != 1)
    {
	fprintf(stderr,"char_graph: error writing labels.\n");
	return (0);
    }
    /*
     * plot points.
     */
    if (char_putxy(x, y, n, x0, y0, x1, y1,
		   (xoff + 1), (yoff + 1), 
		   (maxpx - xoff - 1), (maxpy - yoff - 1), 
		   plot, options) != 1)
    {
	fprintf(stderr,"char_graph: error plotting points.\n");
	return (0);
    }
    /*
     * return to calling program.
     */
    return (1);
}

/*
 * function to return string length.
 */
long char_length(string)

char string[];	/* null-terminated string */
{
    char *pstr;

    pstr = string;
    for ( ; *pstr != ASCIINULL; pstr++)
    {
	/* do nothing */
    }
    return ((long) (pstr - string));
}

