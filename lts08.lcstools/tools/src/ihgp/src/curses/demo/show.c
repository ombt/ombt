#include <curses.h>
#include <signal.h>

main(argc, argv)
int argc;
char **argv;
{
    FILE *fd;
    char linebuf[BUFSIZ];
    int line;
    void done(), perror(), exit();

    if (argc != 2)
    {
	fprintf(stderr,"usage: %s file\n", argv[0]);
	exit(1);
    }
    if ((fd = fopen(argv[1], "r")) == NULL)
    {
	perror(argv[1]);
	exit(2);
    }
    signal(SIGINT, done);
    initscr();
    noecho();
    cbreak();
    nonl();
    idlok(stdscr, TRUE);

    while (1)
    {
	move(0,0);
	for (line = 0; line < LINES; line++)
	{
	    if (fgets(linebuf, sizeof(linebuf), fd) == NULL)
 	    {
		clrtobot();
		done();
	    }
	    move(line, 0);
	    printw("%s", linebuf);
	}
	refresh();
	if (getch() == 'q') done();
    }
}

void done()
{
    move(LINES-1, 0);
    clrtoeol();
    refresh();
    endwin();
    exit(0);
}


