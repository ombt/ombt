#include <sysent.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include "asciiWindow.h"

/* local definitions */
#define DELIMITERS " \t"
#define COMMAND 0
#define ARG1 1
#define ARG2 2
#define ARG3 3
#define ARG4 4
#define ARG5 5
#define ARG6 6
#define ARG7 7
#define ARG8 8
#define ARG9 9
#define MAXIMUMTOKENS 10
#define CHECKWINDOW(pw) ((pw) != (AsciiWindow *)0)

/* globals */
AsciiWindow *window = (AsciiWindow *)0;

/* help function */
void
help()
{
	fprintf(stdout, 
		"clear window - cw x1_coor y1_coor x2_coor y2_coor\n"
		"delete - d\n"
		"display - di\n"
		"draw arc - da center_x_coor center_y_coor\n"
		"           x_intercept y_intercept \n"
		"           arc_offset arc_length color\n"
		"draw box - db x1_coor y1_coor x2_coor y2_coor color\n"
		"draw line - dl x1_coor y1_coor x2_coor y2_coor color\n"
		"fill arc - fa center_x_coor center_y_coor\n"
		"           x_intercept y_intercept arc_offset arc_length\n"
		"           contour_color fill_color\n"
		"fill box - fb x1_coor y1_coor x2_coor y2_coor\n"
		"           contour_color fill_color\n"
		"fill region - fr x_coor y_coor contour_color fill_color\n"
		"get background color - gbc\n"
		"get character - gch\n"
		"get color for point - gcfp x_coor y_coor\n"
		"get cursor - gc\n"
		"get foreground color - gfc\n"
		"get string - gs\n"
		"new - n x_size y_size foreground_color background_color\n"
		"put character - pc x_coor y_coor character color\n"
		"put cursor - pc x_coor y_coor\n"
		"put point - pp x_coor y_coor color\n"
		"put string - ps x_coor y_coor string direction\n"
		"quit - q\n"
		"refresh - r\n"
		"set background color - sbc color\n"
		"set foreground color - sfc color\n"
		"string height - sh string\n"
		"string width - sw string\n");
	return;
}

/* tokenize a character string */
int
tokenize(char *buf, int &ntokens, char **tokens)
{
	/* initialize */
	ntokens = 0;
	tokens[0][0] = (char)0;

	/* tokenize buffer string */
	for (char *ptoken = strtok(buf, DELIMITERS);
	     ptoken != (char *)0 && ntokens < MAXIMUMTOKENS;
	     ntokens++, ptoken = strtok((char *)0, DELIMITERS))
	{
		strcpy(tokens[ntokens], ptoken);
	}

	/* all done */
	return(OK);
}

/* command functions */
int
newwindow(int ntokens, char **tokens)
{
	/* check if window exists */
	if (CHECKWINDOW(window))
	{
		delete window;
		window = (AsciiWindow *)0;
	}

	/* create a new window */
	switch (ntokens)
	{
	case 1:
		window = new AsciiWindow;
		break;
	case 3:
		window = new AsciiWindow(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		break;
	case 4:
		window = new AsciiWindow(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
					long(tokens[ARG3][0]));
		break;
	case 5:
		window = new AsciiWindow(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				long(tokens[ARG3][0]), long(tokens[ARG4][0]));
		break;
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}

	/* check if window was created */
	if ( ! CHECKWINDOW(window))
	{
		errno = ENOMEM;
		ERROR("ENOMEM for creating a window.", errno);
		return(NOTOK);
	}

	/* return status */
	if ((errno = window->getStatus()) != OK) 
		return(NOTOK);
	else
		return(OK);
}

int
deletewindow(int , char **)
{
	/* delete a window */
	if (CHECKWINDOW(window))
	{
		delete window;
		window = (AsciiWindow *)0;
	}
	return(OK);
}

int
putcursor(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* reset cursor */
	switch (ntokens)
	{
	case 3:
		window->putCursor(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
getcursor(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get cursor location */
	int cx, cy;
	window->getCursor(cx, cy);
	if ((errno = window->getStatus()) != OK) return(NOTOK);
	fprintf(stdout, "cursor(x, y) = (%d, %d)\n", cx, cy);
	return(OK);
}

int
putpoint(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* put point on screen */
	switch (ntokens)
	{
	case 1:
		window->putPoint();
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 2:
		window->putPoint(long(tokens[ARG1][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 3:
		window->putPoint(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 4:
		window->putPoint(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				long(tokens[ARG3][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
putcharacter(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* put character on screen */
	switch (ntokens)
	{
	case 2:
		window->putCharacter(int(tokens[ARG1][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 3:
		window->putCharacter(int(tokens[ARG1][0]), long(tokens[ARG2][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 4:
		window->putCharacter(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				int(tokens[ARG3][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 5:
		window->putCharacter(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				int(tokens[ARG3][0]), long(tokens[ARG4][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
putstring(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* put string on screen */
	switch (ntokens)
	{
	case 2:
		window->putString(tokens[ARG1]);
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 3:
		window->putString(tokens[ARG1], 
			(GenericWindow::Direction)atoi(tokens[ARG2]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 4:
		window->putString(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				tokens[ARG3]);
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 5:
		window->putString(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
			tokens[ARG3], 
			(GenericWindow::Direction)atoi(tokens[ARG4]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
stringheight(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get string height */
	switch (ntokens)
	{
	case 2:
		fprintf(stdout, "string height(%s) = %d\n",
			tokens[ARG1], window->stringHeight(tokens[ARG1]));
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
stringwidth(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get string width */
	switch (ntokens)
	{
	case 2:
		fprintf(stdout, "string width(%s) = %d\n",
			tokens[ARG1], window->stringWidth(tokens[ARG1]));
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
getcharacter(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get character */
	int c;
	window->getCharacter(c);
	if ((errno = window->getStatus()) != OK) return(NOTOK);
	fprintf(stdout, "char = %c\n", c);
	return(OK);
}

int
getstring(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get string */
	char buf[BUFSIZ];
	window->getString(BUFSIZ, buf);
	if ((errno = window->getStatus()) != OK) return(NOTOK);
	fprintf(stdout, "string = %s\n", buf);
	return(OK);
}

int
setforegroundcolor(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* set foreground color */
	switch (ntokens)
	{
	case 2:
		window->setForegroundColor(long(tokens[ARG1][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
getforegroundcolor(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get foreground color */
	long fg = window->getForegroundColor();
	if ((errno = window->getStatus()) != OK) return(NOTOK);
	fprintf(stdout, "foreground color = %ld\n", fg);
	return(OK);
}

int
setbackgroundcolor(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* set background color */
	switch (ntokens)
	{
	case 2:
		window->setBackgroundColor(long(tokens[ARG1][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
getbackgroundcolor(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get background color */
	long bg = window->getBackgroundColor();
	if ((errno = window->getStatus()) != OK) return(NOTOK);
	fprintf(stdout, "background color = %ld\n", bg);
	return(OK);
}

int
getcolorforpoint(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get color at point */
	switch (ntokens)
	{
	case 3:
	{
		long pg = window->getColorForPoint(atoi(tokens[ARG1]), 
				atoi(tokens[ARG2]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		fprintf(stdout, "color (%d, %d) = %ld\n", 
			atoi(tokens[ARG1]), atoi(tokens[ARG2]), pg);
		return(OK);
	}
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
clearwindow(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* clear window */
	switch (ntokens)
	{
	case 1:
		window->clearWindow();
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 5:
		window->clearWindow(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
drawline(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* draw a line */
	switch (ntokens)
	{
	case 3:
		window->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 4:
		window->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				long(tokens[ARG3][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 5:
		window->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 6:
		window->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				long(tokens[ARG5][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
fillregion(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* fill a region */
	switch (ntokens)
	{
	case 3:
		window->fillRegion(long(tokens[ARG1][0]), long(tokens[ARG2][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 5:
		window->fillRegion(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				long(tokens[ARG3][0]), long(tokens[ARG4][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
drawarc(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* draw an arc */
	switch (ntokens)
	{
	case 7:
		window->drawArc(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atoi(tokens[ARG5]), atoi(tokens[ARG6]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 8:
		window->drawArc(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atoi(tokens[ARG5]), atoi(tokens[ARG6]),
				long(tokens[ARG7][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
fillarc(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* fill arc */
	switch (ntokens)
	{
	case 9:
		window->fillArc(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atoi(tokens[ARG5]), atoi(tokens[ARG6]),
				long(tokens[ARG7][0]), long(tokens[ARG8][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
drawbox(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* draw a box */
	switch (ntokens)
	{
	case 5:
		window->drawBox(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	case 6:
		window->drawBox(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				long(tokens[ARG5][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
fillbox(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* fill box */
	switch (ntokens)
	{
	case 7:
		window->fillBox(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				long(tokens[ARG5][0]), long(tokens[ARG6][0]));
		if ((errno = window->getStatus()) != OK) return(NOTOK);
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
display(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(window))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* display screen */
	window->display();
	if ((errno = window->getStatus()) != OK) return(NOTOK);
	return(OK);
}

int
refresh(int ntokens, char **tokens)
{
	return(display(ntokens, tokens));
}

/* main entry point */
main(int , char **)
{
	int ntokens;
	char **tokens, buf[BUFSIZ];

	/* allocate token buffer */
	tokens = new char *[MAXIMUMTOKENS];
	if (tokens == (char **)0)
	{
		ERROR("ENOMEM when allocating token buffer.", ENOMEM);
		return(2);
	}
	for (int it = 0; it < MAXIMUMTOKENS; it++)
	{
		tokens[it] = new char [BUFSIZ];
		if (tokens[it] == (char *)0)
		{
			ERROR("ENOMEM when allocating token buffer.", ENOMEM);
			return(2);
		}
	}

	/* get commands from user and execute */
	while ( 1 )
	{
		/* get command */
		gets(buf);
		if (feof(stdin) || ferror(stdin)) break;

		/* split string into tokens */
		if (tokenize(buf, ntokens, tokens) != OK)
		{
			ERROR("unable to tokenize.", errno);
			return(2);
		}

		/* get command */
		if (strcmp(tokens[COMMAND], "n") == 0)
		{
			/* new window */
			if (newwindow(ntokens, tokens) != OK)
			{
				ERROR("new window failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "d") == 0)
		{
			/* delete window */
			if (deletewindow(ntokens, tokens) != OK)
			{
				ERROR("delete window failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "pc") == 0)
		{
			/* put cursor */
			if (putcursor(ntokens, tokens) != OK)
			{
				ERROR("put cursor failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "gc") == 0)
		{
			/* get cursor */
			if (getcursor(ntokens, tokens) != OK)
			{
				ERROR("get cursor failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "pp") == 0)
		{
			/* put point */
			if (putpoint(ntokens, tokens) != OK)
			{
				ERROR("put point failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "pch") == 0)
		{
			/* put character */
			if (putcharacter(ntokens, tokens) != OK)
			{
				ERROR("put character failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "ps") == 0)
		{
			/* put string */
			if (putstring(ntokens, tokens) != OK)
			{
				ERROR("put string failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "sh") == 0)
		{
			/* string height */
			if (stringheight(ntokens, tokens) != OK)
			{
				ERROR("string height failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "sw") == 0)
		{
			/* string width */
			if (stringwidth(ntokens, tokens) != OK)
			{
				ERROR("string width failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "gch") == 0)
		{
			/* get character */
			if (getcharacter(ntokens, tokens) != OK)
			{
				ERROR("get character failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "gs") == 0)
		{
			/* get string */
			if (getstring(ntokens, tokens) != OK)
			{
				ERROR("get string failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "sfc") == 0)
		{
			/* set foreground color */
			if (setforegroundcolor(ntokens, tokens) != OK)
			{
				ERROR("set foreground color failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "gfc") == 0)
		{
			/* get foreground color */
			if (getforegroundcolor(ntokens, tokens) != OK)
			{
				ERROR("get foreground color failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "sbc") == 0)
		{
			/* set background color */
			if (setbackgroundcolor(ntokens, tokens) != OK)
			{
				ERROR("set background color failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "gbc") == 0)
		{
			/* get background color */
			if (getbackgroundcolor(ntokens, tokens) != OK)
			{
				ERROR("get background color failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "gcfp") == 0)
		{
			/* get color for point */
			if (getcolorforpoint(ntokens, tokens) != OK)
			{
				ERROR("get color for point failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "cw") == 0)
		{
			/* clear window */
			if (clearwindow(ntokens, tokens) != OK)
			{
				ERROR("clear window failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "dl") == 0)
		{
			/* draw line */
			if (drawline(ntokens, tokens) != OK)
			{
				ERROR("draw line failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "fr") == 0)
		{
			/* fill region */
			if (fillregion(ntokens, tokens) != OK)
			{
				ERROR("fill region failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "da") == 0)
		{
			/* draw arc */
			if (drawarc(ntokens, tokens) != OK)
			{
				ERROR("draw arc failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "fa") == 0)
		{
			/* fill arc */
			if (fillarc(ntokens, tokens) != OK)
			{
				ERROR("fill arc failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "db") == 0)
		{
			/* draw box */
			if (drawbox(ntokens, tokens) != OK)
			{
				ERROR("draw box failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "fb") == 0)
		{
			/* fill box */
			if (fillbox(ntokens, tokens) != OK)
			{
				ERROR("fill box failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "di") == 0)
		{
			/* display */
			if (display(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "r") == 0)
		{
			/* refresh */
			if (refresh(ntokens, tokens) != OK)
			{
				ERROR("refresh failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "q") == 0)
		{
			/* quit */
			if (window != (AsciiWindow *)0)
			{
				delete window;
				window = (AsciiWindow *)0;
			}
			break;
		}
		else if ((strcmp(tokens[COMMAND], "?") == 0) ||
			 (strcmp(tokens[COMMAND], "help") == 0))
		{
			/* help */
			help();
		}
		else if (tokens[COMMAND][0] != 0)
		{
			ERROR("unrecognized command.", EINVAL);
		}

		/* check for errors */
		if (window != (AsciiWindow *)0 && window->getStatus() != OK)
		{
			ERROR("command error.", window->getStatus());
		}
	}
	return(0);
}
