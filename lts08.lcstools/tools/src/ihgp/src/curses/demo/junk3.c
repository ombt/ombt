#include <curses.h>
#include <signal.h>

main()
{
    WINDOW *win;
    short nl, nc, br, bc;
    char inbuf[100];
    short flag;

    void done();

    /*
     * initialize screen.
     */
    initscr();

    /*
     * exit function.
     */
    signal(SIGINT, done);

    /*
     * get coordinates, and size for window.
     */
    br = 0;		/* window beginning row */
    bc = 0;		/* window beginning column */
    nc = COLS;		/* number of columns in window */
    nl = LINES;		/* number of rows in window */

    /*
     * create window, and box.
     */
    win = newwin(nl, nc, br, bc);
    box(win, '|', '-');	

    /*
     * display windows.
     */
    touchwin(win);
    wrefresh(win);

    /*
     * get input from window.
     */
    scrollok(win, TRUE);

    *inbuf = ! EOF;
    flag = TRUE;
    while (*inbuf != EOF);
    {
	wmove(win, 1, 1);
	wgetstr(win, inbuf);
	wrefresh(win);
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


