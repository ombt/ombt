#include <curses.h>

WINDOW *cmdwin;

main()
{
    int i, c;
    char buf[120];

    initscr();
    nonl();
    noecho();
    cbreak();

    cmdwin = newwin(3, COLS, 0, 0);

    for (i = 0; i < LINES; i++)
    {
	mvprintw(i, 0, "this is line %d of stdscr", i);
    }

    for ( ; ; )
    {
	refresh();
	c = getch();
   	switch (c)
	{
  	case 'c':
	    werase(cmdwin);
	    wprintw(cmdwin,"enter command:");
	    wmove(cmdwin, 2, 0);
	    for (i = 0; i < COLS; i++)
	    {
		waddch(cmdwin, '-');
	    }
	    wmove(cmdwin, 1, 0);
	    touchwin(cmdwin);
	    wrefresh(cmdwin);
	    wgetstr(cmdwin, buf);
 	    touchwin(stdscr);
	    wmove(stdscr, 14, 0);
	    wprintw(stdscr,"command: %s", buf);
	    break;
	case 'q':
	    endwin();
	    exit(0);
	}
    }
}
