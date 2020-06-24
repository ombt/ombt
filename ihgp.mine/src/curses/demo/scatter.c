#include <curses.h>
#include <math.h>

#define MAXLINES 120
#define MAXCOLS 160
char s[MAXLINES][MAXCOLS];	/* screen array */

main()
{
    register int row=0, col=0;
    register char c;
    int char_count=0;
    long t;
    char buf[BUFSIZ];

    initscr();
    for (row=0; row < MAXLINES; row++)
    {
	for (col = 0; col < MAXCOLS; col++)
	{
	    s[row][col] = ' ';
	}
    }
    row = 0;
    while ((c = getchar()) != EOF && row < LINES)
    {
	if (c != '\n')
	{
	    s[row][col++] = c;
	    if (c != ' ') char_count++;
	}
	else
	{
	    col = 0;
	    row++;
	}
    }
    time(&t);
    srand((int) (t & 0177777L));
    while (char_count)
    {
	row = rand() % LINES;
	col = (rand()>>2) % COLS;
	if (s[row][col] != ' ')
	{
	    move(row, col);
	    addch(s[row][col]);
	    s[row][col] = EOF;
	    char_count--;
	    refresh();
	}
    }
    endwin();
    exit(0);
}

