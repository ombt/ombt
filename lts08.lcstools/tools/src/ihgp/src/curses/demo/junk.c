#include <curses.h>

main()
{
    initscr();

    move(LINES/2, 0);
    attrset(A_BOLD);
    printw("lines = %d, columns = %d", LINES, COLS);
    refresh();
	sleep(2);

    endwin();
    exit(0);
}
