#include "/usr/5include/curses.h"
#include <signal.h>

struct screen *me, *you;
struct screen *set_term();

FILE *fd, *fdyou;
char linebuf[BUFSIZ+1];

void
dump_page(term)
struct screen *term;
{
	int line;
	set_term(term);
	for (line=0; line < LINES-1; line++)
	{
		if (fgets(linebuf, BUFSIZ, fd) == NULL)
		{
			clrtobot();
			done();
		}
		mvprintw(line, 0, "%s", linebuf);
	}
	standout();
	mvprintw(LINES-1, 0, "--MORE--");
	standend();
	refresh();
	return;
}

int
done()
{
	set_term(you);
	move(LINES-1, 0);
	clrtoeol();
	refresh();
	endwin();

	set_term(me);
	move(LINES-1, 0);
	clrtoeol();
	refresh();
	endwin();

	exit(0);
}

main(argc, argv)
int argc;
char **argv;
{
	int done();
	int c;

	if (argc != 4)
	{
		fprintf(stderr, "usage: two othertty otherttytype inputfile\n");
		exit(2);
	}

	fd = fopen(argv[3], "r");
	fdyou = fopen(argv[1], "w+");
	signal(SIGINT, done);

	me = newterm(getenv("TERM"), stdout);
	you = newterm(argv[2], fdyou);

	set_term(me);
	noecho();
	cbreak();
	nonl();
	nodelay(stdscr, TRUE);

	set_term(you);
	noecho();
	cbreak();
	nonl();
	nodelay(stdscr, TRUE);

	dump_page(me);
	dump_page(you);

	while ( 1 )
	{
		set_term(me);
		c = getch();
		if (c == 'q') done();
		if (c == ' ') dump_page(me);
		set_term(you);
		c = getch();
		if (c == 'q') done();
		if (c == ' ') dump_page(you);
		sleep(1);
	}
}
