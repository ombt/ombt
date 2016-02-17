#include <curses.h>
#include <signal.h>

#define CTRL(c) ('c' & 037)

main()
{
    WINDOW *uwin, *lwin;
    short unl, unc, ubr, ubc;
    short lnl, lnc, lbr, lbc;
    char inbuf[100];
    short finished;
    int c;

    void done();

    /*
     * initialize screen.
     */
    initscr();
    cbreak();
    noecho();

    /*
     * exit function.
     */
    signal(SIGINT, done);

    /*
     * get coordinates, and sizes for windows.
     */
    ubr = 0;		/* upper window beginning row */
    ubc = 0;		/* upper window beginning column */
    unc = COLS;		/* number of columns in upper window */
    unl = (LINES - 4);	/* number of rows in upper window */

    lbr = LINES - 4;	/* lower window beginning row */
    lbc = 0;		/* lower window beginning column */
    lnc = COLS;		/* number of columns in lower window */
    lnl = 4;		/* number of rows in lower window */

    /*
     * create windows.
     */
    uwin = newwin(unl, unc, ubr, ubc);
    lwin = newwin(lnl, lnc, lbr, lbc);

    /*
     * box windows.
     */
    box(uwin, '*', '*');	/* draw box around upper window */
    box(lwin, '+', '+');	/* draw box around lower window */

    /*
     * display windows.
     */
    touchwin(uwin);		/* display upper window */
    wrefresh(uwin);
    touchwin(lwin);		/* display lower window */
    wrefresh(lwin);

    /*
     * enable scrolling for lower window.
     */
    scrollok(lwin, TRUE);

    /*
     * get input, echo back.
     */
    finished = FALSE;
    wmove(lwin, 1, 1);
    touchwin(lwin);
    wrefresh(lwin);
    while (! finished)
    {
	c = getch();
	switch (c)
	{
	case CTRL(D):
	    finished = TRUE;
	    break;
	default:
	    waddch(lwin, c);
	    break;
	}
	touchwin(lwin);
	wrefresh(lwin);
    }

    /*
     * exit.
     */
    endwin();
    exit(0);
}

void done()
{
    endwin();
    exit(0);
}


