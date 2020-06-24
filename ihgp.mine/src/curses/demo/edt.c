#include <curses.h>

#define CTRL(c) ('c' & 037)

main(argc, argv)
int argc;
char **argv;
{
    int i, n, l;
    int c;
    FILE *fd;

    if (argc != 2)
    {
	fprintf(stderr,"Usage; edit file\n");
	exit(1);
    }

    if ((fd = fopen(argv[1], "r")) == NULL)
    {
	perror(argv[1]);
	exit(2);
    }

    initscr();
    cbreak();
    nonl();
    noecho();
    idlok(stdscr, TRUE);
    keypad(stdscr, TRUE);

    while ((c = getc(fd)) != EOF)
    {
	addch(c);
    }

    move(0,0);
    refresh();
    edit();

    fd = fopen(argv[1], "w");
    for (l = 0; l < 23; l++)
    {
	n = len(l);
	for (i = 0; i < n; i++)
	{
	    putc(mvinch(l,i), fd);
	}
	putc('\n',fd);
    }
    fclose(fd);

    endwin();
    exit();
}

len(lineno)
int lineno;
{
    int linelen = COLS-1;

    while (linelen >= 0 && mvinch(lineno, linelen) == ' ')
	linelen--;
    return(linelen + 1);
}

int row, col;

edit()
{
    int c;

    for ( ; ; )
    {
	move(row, col);
  	refresh();
	c = getch();
	switch (c)
	{
	case 'h':
	case KEY_LEFT:
	    if (col > 0) col--;
	    break;
	case 'j':
	case KEY_DOWN:
	    if (row < LINES-1) row++;
	    break;
	case 'k':
	case KEY_UP:
	    if (row > 0) row--;
	    break;
	case 'l':
	case KEY_RIGHT:
	    if (col < COLS-1) col++;
	    break;
	case KEY_IC:
	case 'i':
	    input();
	    break;
	case KEY_DC:
	case 'x':
	    delch();
	    break;
	case KEY_IL:
	case 'o':
	    move(++row, col=0);
	    insertln();
	    input();
	    break;
	case KEY_DL:
	case 'd':
	    deleteln();
	    break;
	case KEY_CLEAR:
	case CTRL(L):
	    clearok(curscr);
	    refresh();
	    break;
	case 'q':
	    endwin();
	    exit(1);
	default:
	    flash();
	    break;
	}
    }
}

input()
{
    int c;

    standout();
    mvaddstr(LINES-1, COLS-20, "INPUT MODE");
    standend();

    move(row, col);

    refresh();
    for ( ; ; )
    {
	c = getch();
	if (c == CTRL(D) || c == KEY_EIC) break;
	insch(c);
	move(row, ++col);
	refresh();
    }
    move(LINES-1, COLS-20);
    clrtoeol();
    move(row, col);
    refresh();
}

	    
