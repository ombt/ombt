#include <sysent.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include "uniform.h"
#include "xWindow.h"

main(int argc, char **argv)
{
	/* create window display */
	XWindowDisplay *windisp = new XWindowDisplay((char *)0, 0);

	/* create a window */
	XWindow *win = new XWindow(argv[0], windisp, 50, 50, 800, 600,
	"-adobe-courier-bold-o-normal--18-180-75-75-m-110-iso8859-1");

	/* concatenate all strings together */
	char buf[BUFSIZ];
	*buf = 0;
	for (int arg = 1; arg < argc; arg++)
	{
		strcat(buf, " ");
		strcat(buf, argv[arg]);
	}

	/* create random number generators */
	Uniform xdist(1.0, 800.0);
	Uniform ydist(1.0, 600.0);

	/* set seed for random number generator */
	long seed = time((long *)0) | 1L;
	srand48(seed);

	/* write string on screen */
	win->display();
	while ( 1 )
	{
		int x = (int)xdist.randomSample();
		int y = (int)ydist.randomSample();
		win->putString(x, y, buf);
		win->display();
		sleep(1);
	}

	/* all done */
	delete win;
	delete windisp;

	/* all done */	
	return(0);
}
