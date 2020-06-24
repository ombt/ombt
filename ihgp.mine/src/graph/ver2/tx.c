#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "xWindow.h"

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
#define CHECKWINDOW(pw) ((pw) != (XWindow *)0)

/* globals */
XWindow *localWindow = (XWindow *)0;
XWindowDisplay *localWindowDisplay = (XWindowDisplay *)0;

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
		"new - n window_name x_coor y_coor x_size y_size\n"
		"put character - pch x_coor y_coor character color\n"
		"put cursor - pc x_coor y_coor\n"
		"put point - pp x_coor y_coor color\n"
		"put string - ps x_coor y_coor string direction\n"
		"quit - q\n"
		"refresh - r\n"
		"set background color - sbc color\n"
		"set foreground color - sfc color\n"
		"set font fontname - sf fontname\n"
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
#if 0
	for (char *ptoken = strtok(buf, DELIMITERS);
	     ptoken != (char *)0 && ntokens < MAXIMUMTOKENS;
	     ntokens++, ptoken = strtok((char *)0, DELIMITERS))
	{
		strcpy(tokens[ntokens], ptoken);
	}
#else
	char *pbuf = buf;
	char *ptoken = tokens[ntokens];
	*ptoken = 0;
	while (*pbuf != 0 && ntokens < MAXIMUMTOKENS)
	{
		/* check character */
		switch (*pbuf)
		{
		case ' ':
		case '\t':
		case '\n':
			/* ignore white space */
			pbuf++;
			break;
		case '"':
			/* copy quoted string */
			for (pbuf++; *pbuf != 0 && *pbuf != '"'; )
			{
				*ptoken++ = *pbuf++;
			}
			if (*pbuf == '"') pbuf++;
			*ptoken = 0;
			ptoken = tokens[++ntokens];
			*ptoken = 0;
			break;
		default:
			/* store character */
			*ptoken++ = *pbuf++;
			if (*pbuf == ' ' || *pbuf == '\t' || 
			    *pbuf == '\n' || *pbuf == 0)
			{
				*ptoken = 0;
				ptoken = tokens[++ntokens];
				*ptoken = 0;
			}
		}
	}
#endif

	/* all done */
	return(OK);
}

/* command functions */
int
newwindow(int ntokens, char **tokens)
{
	/* check if window exists */
	if (CHECKWINDOW(localWindow))
	{
		delete localWindow;
		localWindow = (XWindow *)0;
	}

	/* create a new window */
	switch (ntokens)
	{
	case 6:
		localWindow = new XWindow(tokens[ARG1], localWindowDisplay, 
				atoi(tokens[ARG2]), atoi(tokens[ARG3]),
				atoi(tokens[ARG4]), atoi(tokens[ARG5]));
		break;
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}

	/* check if window was created */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = ENOMEM;
		ERROR("ENOMEM for creating a window.", errno);
		return(NOTOK);
	}

	/* return status */
	if ((errno = localWindow->getStatus()) != OK) 
	{
		ERROR("new window constructor failed.", errno);
		return(NOTOK);
	}
	else
		return(OK);
}

int
deletewindow(int , char **)
{
	/* delete a window */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}
	else
	{
		delete localWindow;
		localWindow = (XWindow *)0;
		return(OK);
	}
}

int
putcursor(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* reset cursor */
	switch (ntokens)
	{
	case 3:
		localWindow->putCursor(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put cursor failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get cursor location */
	int cx, cy;
	localWindow->getCursor(cx, cy);
	if ((errno = localWindow->getStatus()) != OK)
	{
		ERROR("get cursor failed.", errno);
		return(NOTOK);
	}
	fprintf(stdout, "cursor(x, y) = (%d, %d)\n", cx, cy);
	return(OK);
}

int
putpoint(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* put point on screen */
	switch (ntokens)
	{
	case 1:
		localWindow->putPoint();
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put point failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 2:
		localWindow->putPoint(atol(tokens[ARG1]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put point failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 3:
		localWindow->putPoint(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put point failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 4:
		localWindow->putPoint(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atol(tokens[ARG3]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put point failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* put character on screen */
	switch (ntokens)
	{
	case 2:
		localWindow->putCharacter(int(tokens[ARG1][0]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put character failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 3:
		localWindow->putCharacter(int(tokens[ARG1][0]), 
					atol(tokens[ARG2]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put character failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 4:
		localWindow->putCharacter(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				int(tokens[ARG3][0]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put character failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 5:
		localWindow->putCharacter(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				int(tokens[ARG3][0]), atol(tokens[ARG4]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put character failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* put string on screen */
	switch (ntokens)
	{
	case 2:
		localWindow->putString(tokens[ARG1]);
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put string failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 3:
		localWindow->putString(tokens[ARG1], 
			(GenericWindow::Direction)atoi(tokens[ARG2]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put string failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 4:
		localWindow->putString(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				tokens[ARG3]);
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put string failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 5:
		localWindow->putString(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
			tokens[ARG3], 
			(GenericWindow::Direction)atoi(tokens[ARG4]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("put string failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
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
			tokens[ARG1], localWindow->stringHeight(tokens[ARG1]));
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
	if ( ! CHECKWINDOW(localWindow))
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
			tokens[ARG1], localWindow->stringWidth(tokens[ARG1]));
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get character */
	int c;
	localWindow->getCharacter(c);
	if ((errno = localWindow->getStatus()) != OK)
	{
		ERROR("get character string failed.", errno);
		return(NOTOK);
	}
	fprintf(stdout, "char = %c\n", c);
	return(OK);
}

int
getstring(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get string */
	char buf[BUFSIZ];
	localWindow->getString(20, buf);
	if ((errno = localWindow->getStatus()) != OK)
	{
		ERROR("get string string failed.", errno);
		return(NOTOK);
	}
	fprintf(stdout, "string = %s\n", buf);
	return(OK);
}

int
setfont(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* set font */
	switch (ntokens)
	{
	case 2:
		localWindow->setFont(tokens[ARG1]);
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("set font failed.", errno);
			return(NOTOK);
		}
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
setforegroundcolor(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* set foreground color */
	switch (ntokens)
	{
	case 2:
		localWindow->setForegroundColor(atol(tokens[ARG1]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("set foreground color failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get foreground color */
	long fg = localWindow->getForegroundColor();
	if ((errno = localWindow->getStatus()) != OK)
	{
		ERROR("get foreground color failed.", errno);
		return(NOTOK);
	}
	fprintf(stdout, "foreground color = %ld\n", fg);
	return(OK);
}

int
setbackgroundcolor(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* set background color */
	switch (ntokens)
	{
	case 2:
		localWindow->setBackgroundColor(atol(tokens[ARG1]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("set background color failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* get background color */
	long bg = localWindow->getBackgroundColor();
	if ((errno = localWindow->getStatus()) != OK)
	{
		ERROR("get background color failed.", errno);
		return(NOTOK);
	}
	fprintf(stdout, "background color = %ld\n", bg);
	return(OK);
}

int
getcolorforpoint(int ntokens, char **tokens)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
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
		long pg = localWindow->getColorForPoint(atoi(tokens[ARG1]), 
				atoi(tokens[ARG2]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("get color for point failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* clear window */
	switch (ntokens)
	{
	case 1:
		localWindow->clearWindow();
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("clear window failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 5:
		localWindow->clearWindow(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("clear window failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* draw a line */
	switch (ntokens)
	{
	case 3:
		localWindow->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw line failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 4:
		localWindow->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atol(tokens[ARG3]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw line failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 5:
		localWindow->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw line failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 6:
		localWindow->drawLine(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atol(tokens[ARG5]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw line failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* draw an arc */
	switch (ntokens)
	{
	case 7:
		localWindow->drawArc(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atoi(tokens[ARG5]), atoi(tokens[ARG6]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw arc failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 8:
		localWindow->drawArc(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atoi(tokens[ARG5]), atoi(tokens[ARG6]),
				atol(tokens[ARG7]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw arc failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* fill arc */
	switch (ntokens)
	{
	case 9:
		localWindow->fillArc(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atoi(tokens[ARG5]), atoi(tokens[ARG6]),
				atol(tokens[ARG7]), atol(tokens[ARG8]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("fill arc failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* draw a box */
	switch (ntokens)
	{
	case 5:
		localWindow->drawBox(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw box failed.", errno);
			return(NOTOK);
		}
		return(OK);
	case 6:
		localWindow->drawBox(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atol(tokens[ARG5]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("draw box failed.", errno);
			return(NOTOK);
		}
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
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* fill box */
	switch (ntokens)
	{
	case 7:
		localWindow->fillBox(atoi(tokens[ARG1]), atoi(tokens[ARG2]),
				atoi(tokens[ARG3]), atoi(tokens[ARG4]),
				atol(tokens[ARG5]), atol(tokens[ARG6]));
		if ((errno = localWindow->getStatus()) != OK)
		{
			ERROR("fill box failed.", errno);
			return(NOTOK);
		}
		return(OK);
	default:
		errno = EINVAL;
		ERROR("invalid number of arguments.", errno);
		return(NOTOK);
	}
}

int
mydisplay(int , char **)
{
	/* check if window exists */
	if ( ! CHECKWINDOW(localWindow))
	{
		errno = EINVAL;
		ERROR("window not allocated.", errno);
		return(NOTOK);
	}

	/* display screen */
	localWindow->display();
	if ((errno = localWindow->getStatus()) != OK)
	{
		ERROR("display box failed.", errno);
		return(NOTOK);
	}
	return(OK);
}

int
refresh(int ntokens, char **tokens)
{
	return(mydisplay(ntokens, tokens));
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

	/* connect to X-window display */
	localWindowDisplay = new XWindowDisplay((char *)0, 0);
	if (localWindowDisplay == (XWindowDisplay *)0)
	{
		ERROR("ENOMEM when allocating window display.", ENOMEM);
		return(2);
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
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
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
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
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
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "pch") == 0)
		{
			/* put character */
			if (putcharacter(ntokens, tokens) != OK)
			{
				ERROR("put character failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "ps") == 0)
		{
			/* put string */
			if (putstring(ntokens, tokens) != OK)
			{
				ERROR("put string failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
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
		else if (strcmp(tokens[COMMAND], "sf") == 0)
		{
			/* set font */
			if (setfont(ntokens, tokens) != OK)
			{
				ERROR("set font failed.", errno);
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
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "dl") == 0)
		{
			/* draw line */
			if (drawline(ntokens, tokens) != OK)
			{
				ERROR("draw line failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "da") == 0)
		{
			/* draw arc */
			if (drawarc(ntokens, tokens) != OK)
			{
				ERROR("draw arc failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "fa") == 0)
		{
			/* fill arc */
			if (fillarc(ntokens, tokens) != OK)
			{
				ERROR("fill arc failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "db") == 0)
		{
			/* draw box */
			if (drawbox(ntokens, tokens) != OK)
			{
				ERROR("draw box failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "fb") == 0)
		{
			/* fill box */
			if (fillbox(ntokens, tokens) != OK)
			{
				ERROR("fill box failed.", errno);
			}
			else if (mydisplay(ntokens, tokens) != OK)
			{
				ERROR("display failed.", errno);
			}
		}
		else if (strcmp(tokens[COMMAND], "di") == 0)
		{
			/* display */
			if (mydisplay(ntokens, tokens) != OK)
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
			if (localWindow != (XWindow *)0)
			{
				delete localWindow;
				localWindow = (XWindow *)0;
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
		if (localWindow != (XWindow *)0 && localWindow->getStatus() != OK)
		{
			ERROR("command error.", localWindow->getStatus());
		}
	}
	return(0);
}
