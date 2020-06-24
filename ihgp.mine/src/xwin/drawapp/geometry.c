/* headers */
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* get and set geometry from argv */
int
FindGeometry(argc, argv, geometry)
int argc;
char **argv;
char *geometry;
{
	char *ptr;

	/* find parameter */
	ptr = FindParameter(argc, argv, "-geom");
	if (ptr != (char *)NULL)
	{
		strcpy(geometry, ptr);
		return(True);
	}
	else
	{
		*geometry = 0;
		return(False);
	}
}

/* parse geometry string */
void
ParseGeometry(geometry, max_width, max_height, x, y, width, height)
char *geometry;
int max_width, max_height;
int *x, *y, *width, *height;
{
	int status;
	int x1, y1, width1, height1;

	/* parse geometry string */
	status = XParseGeometry(geometry, &x1, &y1, &width1, &height1);
	if (status & XValue) *x = x1;
	if (status & YValue) *y = y1;
	if (status & WidthValue) *width = width1;
	if (status & HeightValue) *height = height1;
	if (status & XNegative) *x = max_width - *width + *x;
	if (status & YNegative) *y = max_height - *height + *y;

	/* all done */
	return;
}

/* check if any geometry parameter was given */
void
CheckGeometry(argc, argv, max_width, max_height, x, y, width, height)
int argc;
char **argv;
int max_width, max_height;
int *x, *y, *width, *height;
{
	char geometry[BUFSIZ];

	/* get geometry from command line */
	if (FindGeometry(argc, argv, geometry) == True)
	{
		/* geometry was given */
		ParseGeometry(geometry, max_width, max_height,
			x, y, width, height);
	}

	/* all done */
	return;
}

