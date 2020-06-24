/*
 * interactive plotting program using curses package, and gray
 * levels (curplot.c).
 */

/*
 * standard header files.
 */
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <math.h>
#include "curplot.h"

/* some basic macroes */
#define is_digit(x) ((((x) >= '0') && ((x) <= '9')) ? 1 : 0)
#define is_lowercase(x) ((((x) >= 'a') && ((x) <= 'z')) ? 1 : 0)
#define is_uppercase(x) ((((x) >= 'A') && ((x) <= 'Z')) ? 1 : 0)
#define is_blank(x) (((x) == ' ') ? 1 : 0)

#define to_uppercase(x) ((is_lowercase(x)) ? ((x) - 'a' + 'A') : (x))
#define to_lowercase(x) ((is_uppercase(x)) ? ((x) + 'a' - 'A') : (x))
#define to_decimal(x) ((x) - '0')
#define to_ascii(x) ((((x) >= 0) && ((x) <= 9)) ? ((x) + '0') : -1)

/*
 * commands structure.
 */
struct COMMANDS 
{
    char com[15];
    long key;
};

/*
 * encoding states.
 */
#define CHAR_S 0L
#define GRAY_S 1L

/*
 * keys for commands.
 */
#define BLANKLINE -3L
#define TOOMANY -2L
#define NOMATCH -1L

/*
 * last command.
 */
#define LASTCOM '\0'
#define LASTCOMKEY -1L

/*
 * upper level commands.
 */
#define L1_SET 		0L
#define L1_GET 		L1_SET + 1L
#define L1_PUT 		L1_GET + 1L
#define L1_LINE 	L1_PUT + 1L
#define L1_WINDOW 	L1_LINE + 1L
#define L1_ERASE 	L1_WINDOW + 1L
#define L1_FILL 	L1_ERASE + 1L
#define L1_QUIT 	L1_FILL + 1L
#define L1_HELP 	L1_QUIT + 1L
#define L1_MOVE 	L1_HELP + 1L
#define L1_EXECUTE 	L1_MOVE + 1L
#define L1_SHELL 	L1_EXECUTE + 1L
#define L1_COMMENT 	L1_SHELL + 1L
#define L1_ECHO 	L1_COMMENT + 1L
#define L1_ARC 		L1_ECHO + 1L
#define L1_CLEAR	L1_ARC + 1L
#define L1_FILEIN	L1_CLEAR + 1L
#define L1_FILEOUT	L1_FILEIN + 1L
#define L1_CHANGE	L1_FILEOUT + 1L
#define L1_ENCODE	L1_CHANGE + 1L
#define L1_DISPLAY	L1_ENCODE + 1L

static struct COMMANDS comlev1[] = 
{
    {"SET", L1_SET},
    {"GET", L1_GET},
    {"PUT", L1_PUT},
    {"LINE", L1_LINE},
    {"WINDOW", L1_WINDOW},
    {"ERASE", L1_ERASE},
    {"FILL", L1_FILL},
    {"QUIT", L1_QUIT},
    {"HELP", L1_HELP},
    {"MOVE", L1_MOVE},
    {"EXECUTE", L1_EXECUTE},
    {"SHELL", L1_SHELL},
    {"#", L1_COMMENT},
    {"ECHO", L1_ECHO},
    {"ARC", L1_ARC},
    {"CLEAR", L1_CLEAR},
    {"<", L1_FILEIN},
    {">", L1_FILEOUT},
    {"CHANGE", L1_CHANGE},
    {"ENCODE", L1_ENCODE},
    {"DISPLAY", L1_DISPLAY},
    {"?", L1_HELP},		/* duplicate commands at end */
    {"!", L1_SHELL},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at DISPLAY level.
 */
#define DISPLAY_NO 0L
#define DISPLAY_YES 1L

static struct COMMANDS comdisp[] = 
{
    {"NO", DISPLAY_NO},
    {"YES", DISPLAY_YES},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at SET level.
 */
#define SET_BACKGROUND 0L
#define SET_FOREGROUND 1L
#define SET_CURSOR 2L
#define SET_DEFAULT 3L

static struct COMMANDS comset[] = 
{
    {"BACKGROUND", SET_BACKGROUND},
    {"FOREGROUND", SET_FOREGROUND},
    {"CURSOR", SET_CURSOR},
    {"DEFAULT", SET_DEFAULT},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at GET level.
 */
#define GET_BACKGROUND 0L
#define GET_FOREGROUND 1L
#define GET_CURSOR 2L
#define GET_COLOR 3L
#define GET_SCREEN 4L

static struct COMMANDS comget[] = 
{
    {"BACKGROUND", GET_BACKGROUND},
    {"FOREGROUND", GET_FOREGROUND},
    {"CURSOR", GET_CURSOR},
    {"COLOR", GET_COLOR},
    {"SCREEN", GET_SCREEN},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at PUT level.
 */
#define PUT_SYMBOL 0L
#define PUT_POINT 1L
#define PUT_STRING 2L

static struct COMMANDS comput[] = 
{
    {"SYMBOL", PUT_SYMBOL},
    {"POINT", PUT_POINT},
    {"STRING", PUT_STRING},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at LINE level.
 */
#define LINE_RELATIVE 0L
#define LINE_ABSOLUTE 1L

static struct COMMANDS comline[] = 
{
    {"RELATIVE", LINE_RELATIVE},
    {"ABSOLUTE", LINE_ABSOLUTE},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at ECHO level.
 */
#define ECHO_YES 0L
#define ECHO_NO 1L
#define ECHO_WAIT 2L

static struct COMMANDS comecho[] = 
{
    {"YES", ECHO_YES},
    {"NO", ECHO_NO},
    {"WAIT", ECHO_WAIT},
    {LASTCOM, LASTCOMKEY}
};

/*
 * exit function.
 */
void
good_bye(outsig, outlu, exitmsg)
long outsig;
FILE *outlu;
char exitmsg[];
{
    cur_exitscr();
    if (outlu != (FILE *)NULL) fprintf(outlu,"%s\n", exitmsg);
    exit((int)outsig);
}


/*
 * get input string.
 */
char
*char_gets(input, nchar, stream)
char input[];
long nchar;
FILE *stream;
{
    char *pstr;

    pstr = input;
    if (fgets(input, nchar, stream) == NULL) return (NULL);
    for ( ; (*pstr != '\n') && (*pstr != '\0'); pstr++)
    {
	/* do nothing */
    }
    *pstr = '\0';
    return (input);
}

/*
 * function to identify a command.
 */
int
get_comm(commands, nchar, string)
struct COMMANDS commands[];
long nchar;
char *string[];
{
    long i, match;
    char *pstr, *pstrend, *pcom, *pnext;
    long number_matched;

    for ( ; **string == ' '; ++*string)
    {
	/* do nothing */
    }
    pstrend = *string;
    for ( ; (*pstrend != ' ') && (*pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    pnext = pstrend;		/* next token */
    if (pstrend == *string)
    {
	/* blank line */
	return (BLANKLINE);
    }
    if ((pstrend - *string) > nchar) pstrend = *string + nchar;
    number_matched = 0;
    for (i = 0; commands[i].com[0] != LASTCOM; i++)
    {
	pcom = commands[i].com;
	pstr = *string;
	for ( ; (*pcom == to_uppercase(*pstr)) && 
	        (pstr < pstrend); pstr++, pcom++)
	{
	    /* do nothing */
	}
	if (pstr == pstrend) 
	{
	    number_matched++;
	    match = i;
	}
    }
    switch (number_matched)
    {
    case 0L:
	return (NOMATCH);
    case 1L:
 	*string = pnext;	/* next token in string */
	return (commands[match].key);
    }
    return (TOOMANY);
}


/*
 * set function.
 */
int
c_set(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    /*
     * commands at SET level.
     */
    long status, key;
   
    switch ((key = get_comm(comset, 4L, string)))
    {
    case SET_BACKGROUND:
	if (! (status = set_background(window, string)))
	{
	    wprintw(inw,"SET: background error.\n");
	}
	break;
    case SET_FOREGROUND:
	if (! (status = set_foreground(window, string)))
	{
	    wprintw(inw,"SET: foreground error.\n");
	}
	break;
    case SET_CURSOR:
	if (! (status = set_cursor(window, string)))
	{
	    wprintw(inw,"SET: cursor error.\n");
	}
	break;
    case SET_DEFAULT:
	if (! (status = set_default(window, string)))
	{
	    wprintw(inw,"SET: default error.\n");
	}
	break;
    default:
	status = 0;
	wprintw(inw,"SET: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * get function.
 */
int
c_get(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    /*
     * commands at GET level.
     */
    long status, key;

    switch ((key = get_comm(comget, 4L, string)))
    {
    case GET_BACKGROUND:
	if (! (status = get_background(window, inw, string)))
	{
	    wprintw(inw,"GET: background error.\n");
	}
	break;
    case GET_FOREGROUND:
	if (! (status = get_foreground(window, inw, string)))
	{
	    wprintw(inw,"GET: foreground error.\n");
	}
	break;
    case GET_CURSOR:
	if (! (status = get_cursor(window, inw, string)))
	{
	    wprintw(inw,"GET: cursor error.\n");
	}
	break;
    case GET_COLOR:
	if (! (status = get_color(window, inw, string)))
	{
	    wprintw(inw,"GET: color error.\n");
	}
	break;
    case GET_SCREEN:
	if (! (status = get_screen(window, inw, string)))
	{
	    wprintw(inw,"GET: screen error.\n");
	}
	break;
    default:
	status = 0;
	wprintw(inw,"GET: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * put function.
 */
int
c_put(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    /*
     * commands at PUT level.
     */
    long status, key;

    switch ((key = get_comm(comput, 4L, string)))
    {
    case PUT_SYMBOL:
	if (! (status = put_symbol(window, string)))
	{
	    wprintw(inw,"PUT: symbol error.\n");
	}
	break;
    case PUT_POINT:
	if (! (status = put_point(window, string)))
	{
	    wprintw(inw,"PUT: point error.\n");
	}
	break;
    case PUT_STRING:
	if (! (status = put_string(window, string)))
	{
	    wprintw(inw,"PUT: string error.\n");
	}
	break;
    default:
	status = 0;
	wprintw(inw,"PUT: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * set background function.
 */
int
set_background(window, string)
CUR_WINDOW *window;
char *string[];
{
    char color;
    short nchar;

#ifdef DEBUG
    fprintf(stdout,"set background: %s.\n",*string);
#endif
    nchar = sscanf(*string, "%1s", &color);
    switch (nchar)
    {
    case 1:
	break;
    default:
	color = ' ';
	break;
    }
    if (! cur_setback(window, &color)) return (0);
    return (1);
}

/*
 * set foreground function.
 */
int
set_foreground(window, string)
CUR_WINDOW *window;
char *string[];
{
    char color;
    short nchar;
    long level;

#ifdef DEBUG
    fprintf(stdout,"set foreground: %s.\n",*string);
#endif

    nchar = sscanf(*string, "%d %1s", &level, &color);
    switch (nchar)
    {
    case 2:
	break;
    case 1:
	color = ' ';
	break;
    default:
	return (0);
    }
    if (! cur_setfore(window, &color, level)) return (0);
    return (1);
}

/*
 * set cursor function.
 */
int
set_cursor(window, string)
CUR_WINDOW *window;
char *string[];
{
    long px, py;

#ifdef DEBUG
    fprintf(stdout,"set cursor: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &px, &py) != 2) return (0);
    if (! cur_setcursor(window, px, py)) return (0);
    return (1);
}

/*
 * set default colors function.
 */
int
set_default(window, string)
CUR_WINDOW *window;
char *string[];
{
    char color;
    long level;

#ifdef DEBUG
    fprintf(stdout,"set default: %s.\n",*string);
#endif

    for (level = 0; level <= MAXCOLOR; level++)
    {
	if (level < 10)
	{
	    color = (UCHAR) (level + '0');
	}
	else
	{
	    color = (UCHAR) (level - 10 + 'A');
  	}
	if (! cur_setfore(window, &color, level)) return (0);
    }
    return (1);
}

/*
 * get background function.
 */
int
get_background(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    char color;

#ifdef DEBUG
    fprintf(stdout,"get background: %s.\n",*string);
#endif
    if (! cur_getback(window, &color)) return (0);
    wprintw(inw,"background color: %c\n", color);
    return (1);
}

/*
 * get foreground function.
 */
int
get_foreground(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    char color;
    long level;

#ifdef DEBUG
    fprintf(stdout,"get foreground: %s.\n",*string);
#endif
    if (sscanf(*string, "%d", &level) != 1) return (0);
    if (! cur_getfore(window, &color, level)) return (0);
    wprintw(inw,"foreground color for level %d: %c\n", level, color);
    return (1);
}

/*
 * get cursor function.
 */
int
get_cursor(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long px, py;

#ifdef DEBUG
    fprintf(stdout,"get cursor: %s.\n",*string);
#endif
    if (! cur_getcursor(window, &px, &py)) return (0);
    wprintw(inw,"cursor position (x,y) = (%d,%d)\n", px, py);
    return (1);
}

/*
 * get color function (return ASCII or gray level).
 */
int
get_color(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long px, py;
    long level;
    char ascii;

#ifdef DEBUG
    fprintf(stdout,"get color: %s.\n",*string);
#endif
    if (! cur_getcursor(window, &px, &py)) return (0);
    switch (cur_pointcolor(window, px, py, &level, &ascii))
    {
    case ASCII:
	wprintw(inw, "character at (%d,%d): %c\n", px, py, ascii);
	break;
    case GRAY:
	wprintw(inw, "gray level at (%d,%d): %d\n", px, py, level);
	break;
    default:
	return (0);
    }
    return (1);
}

/*
 * get screen size function.
 */
int
get_screen(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long px, py;

#ifdef DEBUG
    fprintf(stdout,"get screen: %s.\n",*string);
#endif
    wprintw(inw,"screen size (x,y) = (%d,%d)\n", 
	    window->maxpx, window->maxpy);
    return (1);
}

/*
 * put symbol function.
 */
int
put_symbol(window, string)
CUR_WINDOW *window;
char *string[];
{
    char *pstr;

#ifdef DEBUG
    fprintf(stdout,"put symbol: %s.\n",*string);
#endif
    pstr = *string;
    for ( ; (*pstr == ' ') && (*pstr != '\0'); pstr++)
    {
	/* do nothing */
    }
    if (! cur_putsym(window, pstr)) return (0);
    return (1);
}

/*
 * put point function.
 */
int
put_point(window, string)
CUR_WINDOW *window;
char *string[];
{
    long level;

#ifdef DEBUG
    fprintf(stdout,"put point: %s.\n",*string);
#endif
    if (sscanf(*string, "%d", &level) != 1) return (0);
    if (! cur_putpoint(window, level)) return (0);
    return (1);
}

/*
 * put string function.
 */
int
put_string(window, string)
CUR_WINDOW *window;
char *string[];
{
    char *pstr;
    long flag;

#ifdef DEBUG
    fprintf(stdout,"put string: %s.\n",*string);
#endif
    if (sscanf(*string, "%d", &flag) != 1) return (0);
    pstr = *string;
    for ( ; (*pstr == ' ') && (*pstr != '\0'); pstr++)
    {
	/* do nothing */
    }
    for ( ; is_digit(*pstr) && (*pstr != '\0'); pstr++)
    {
	/* do nothing */
    }
    if (! cur_putstring(window, pstr, flag)) return (0);
    return (1);
}

/*
 * line function.
 */
int
c_line(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    /*
     * commands at LINE level.
     */
    long key;

    switch ((key = get_comm(comline, 4L, string)))
    {
    case LINE_RELATIVE:
	if (! line_relative(window, string))
	{
	    wprintw(inw,"LINE: relative error.\n");
	}
	break;
    case LINE_ABSOLUTE:
    default:
	if (! line_absolute(window, string))
	{
	    wprintw(inw,"LINE: absolute error.\n");
	}
	break;
    }
    return (1);
}

/*
 * line absolute function.
 */
int
line_absolute(window, string)
CUR_WINDOW *window;
char *string[];
{
    long px, py, level;

#ifdef DEBUG
    fprintf(stdout,"line absolute: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d %d", &px, &py, &level) != 3) return (0);
    if (! cur_putline(window, px, py, level)) return (0);
    return (1);
}

/*
 * line relative function.
 */
int
line_relative(window, string)
CUR_WINDOW *window;
char *string[];
{
    long dpx, dpy, px, py, level;

#ifdef DEBUG
    fprintf(stdout,"line relative: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d %d", &dpx, &dpy, &level) != 3) return (0);
    if (! cur_getcursor(window, &px, &py)) return (0);
    if (! cur_putline(window, (px + dpx), (py + dpy), level)) return (0);
    return (1);
}

/*
 * window function.
 */
int
c_window(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long px0, py0, px1, py1, level;

#ifdef DEBUG
    fprintf(stdout,"window: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d %d %d %d", 
	       &px0, &py0, &px1, &py1, &level) != 5) return (0);
    if (! cur_paintw(window, px0, py0, px1, py1, level)) return (0);
    return (1);
}

/*
 * erase function.
 */
int
c_erase(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
#ifdef DEBUG
    fprintf(stdout,"erase: %s.\n",*string);
#endif
    if (! cur_erase(window)) return (0);
    return (1);
}

/*
 * fill function, assume over write ASCII.
 */
int
c_fill(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long px, py, level, contour;

#ifdef DEBUG
    fprintf(stdout,"fill: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &level, &contour) != 2) return (0);
    if (! cur_getcursor(window, &px, &py)) return (0);
    cur_fill(window, px, py, level, contour, OWASCII);
    return (1);
}

/*
 * help function.
 */
int
c_help(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long key;
    WINDOW *helpw;
    int c, nr, nc, br, bc;

    br = 0;
    bc = 0;
    nc = COLS;
    nr = ((15 < LINES) ? 15 : LINES);

    helpw = newwin(nr, nc, br, bc);
    if (helpw == (WINDOW *) NULL) return(0);

    idlok(helpw, TRUE);			/* enable hardware scrolling */
    scrollok(helpw, TRUE);		/* enable scrolling */
    wsetscrreg(helpw, 0, (nr - 1));	/* where to scroll */

    wmove(helpw, 0, 0);			/* move cursor to (0,0) */
    
    wattron(helpw, A_STANDOUT);
    switch ((key = get_comm(comlev1, 4L, string)))
    {
    case L1_SET:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comset, 4L, string)))
	{
	case SET_BACKGROUND:
	    wprintw(helpw,"Usage: SET BACKGROUND color\n");
	    break;
	case SET_FOREGROUND:
	    wprintw(helpw,"Usage: SET FOREGROUND level color\n");
	    break;
	case SET_CURSOR:
	    wprintw(helpw,"Usage: SET CURSOR px py\n");
	    break;
	case SET_DEFAULT:
	    wprintw(helpw,"Usage: SET DEFAULT\n");
	    break;
	default:
	    wprintw(helpw,"Usage: SET BACKGROUND color\n");
	    wprintw(helpw,"Usage: SET FOREGROUND level color\n");
	    wprintw(helpw,"Usage: SET CURSOR px py\n");
	    wprintw(helpw,"Usage: SET DEFAULT\n");
	    break;
	}
	break;
    case L1_GET:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comget, 4L, string)))
	{
	case GET_BACKGROUND:
	    wprintw(helpw,"Usage: GET BACKGROUND\n");
	    break;
	case GET_FOREGROUND:
	    wprintw(helpw,"Usage: GET FOREGROUND level\n");
	    break;
	case GET_CURSOR:
	    wprintw(helpw,"Usage: GET CURSOR\n");
	    break;
	case GET_COLOR:
	    wprintw(helpw,"Usage: GET COLOR\n");
	    break;
	case GET_SCREEN:
	    wprintw(helpw,"Usage: GET SCREEN\n");
	    break;
	default:
	    wprintw(helpw,"Usage: GET BACKGROUND\n");
	    wprintw(helpw,"Usage: GET FOREGROUND level\n");
	    wprintw(helpw,"Usage: GET CURSOR\n");
	    wprintw(helpw,"Usage: GET COLOR\n");
	    wprintw(helpw,"Usage: GET SCREEN\n");
	    break;
	}
	break;
    case L1_PUT:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comput, 4L, string)))
	{
	case PUT_SYMBOL:
	    wprintw(helpw,"Usage: PUT SYMBOL character\n");
	    break;
	case PUT_POINT:
	    wprintw(helpw,"Usage: PUT POINT level\n");
	    break;
	case PUT_STRING:
	    wprintw(helpw,"Usage: PUT STRING direction character_string\n");
	    break;
	default:
	    wprintw(helpw,"Usage: PUT SYMBOL character\n");
	    wprintw(helpw,"Usage: PUT POINT level\n");
	    wprintw(helpw,"Usage: PUT STRING direction character_string\n");
	    break;
	}
	break;
    case L1_LINE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comline, 4L, string)))
	{
	case LINE_RELATIVE:
	    wprintw(helpw,"Usage: LINE RELATIVE dpx dpy level\n");
	    break;
	case LINE_ABSOLUTE:
	    wprintw(helpw,"Usage: LINE [ABSOLUTE] px py level\n");
	    break;
	default:
	    wprintw(helpw,"Usage: LINE RELATIVE dpx dpy level\n");
	    wprintw(helpw,"Usage: LINE [ABSOLUTE] px py level\n");
	    break;
	}
	break;
    case L1_WINDOW:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: WINDOW px0 py0 px1 py1 level\n");
	break;
    case L1_ERASE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: ERASE\n");
	break;
    case L1_FILL:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: FILL fill_level contour_level\n");
	break;
    case L1_QUIT:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: QUIT\n");
	break;
    case L1_HELP:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: HELP [command[s]]\n");
	wprintw(helpw,"Usage: ? [command[s]]\n");
	break;
    case L1_MOVE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: MOVE dpx dpy\n");
	break;
    case L1_EXECUTE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: EXEC commands_file\n");
	break;
    case L1_SHELL:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: SHELL command\n");
	wprintw(helpw,"Usage: ! command\n");
	break;
    case L1_COMMENT:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: # [comment[s]]\n");
	break;
    case L1_CLEAR:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: CLEAR\n");
	break;
    case L1_ECHO:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comecho, 4L, string)))
	{
	case ECHO_YES:
	    wprintw(helpw,"Usage: ECHO YES\n");
	    break;
	case ECHO_NO:
	    wprintw(helpw,"Usage: ECHO NO\n");
	    break;
	case ECHO_WAIT:
	    wprintw(helpw,"Usage: ECHO WAIT\n");
	    break;
	default:
	    wprintw(helpw,"Usage: ECHO YES\n");
	    wprintw(helpw,"Usage: ECHO NO\n");
	    wprintw(helpw,"Usage: ECHO WAIT\n");
	    break;
	}
	break;
    case L1_DISPLAY:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comdisp, 4L, string)))
	{
	case DISPLAY_YES:
	    wprintw(helpw,"Usage: DISPLAY YES\n");
	    break;
	case DISPLAY_NO:
	    wprintw(helpw,"Usage: DISPLAY NO\n");
	    break;
	default:
	    wprintw(helpw,"Usage: DISPLAY YES\n");
	    wprintw(helpw,"Usage: DISPLAY NO\n");
	    break;
	}
	break;
    case L1_ARC:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: ARC radius alpha arc level\n");
	break;
    case L1_FILEIN:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: < file name\n");
	break;
    case L1_FILEOUT:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: > file name\n");
	break;
    case L1_CHANGE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: CHANGE oldlevel newlevel\n");
	break;
    case L1_ENCODE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	wprintw(helpw,"Usage: ENCODE file name\n");
	break;
    default:
	wprintw(helpw,"List of commands\n");
	wprintw(helpw,"----------------\n");
	for (key = 0; comlev1[key].com[0] != LASTCOM; key++)
	{
	    wprintw(helpw,"%s ",comlev1[key].com);
	    if (((key % 4) == 0) && (key != 0)) wprintw(helpw,"\n");
	}
	wprintw(helpw,"\n");
	wprintw(helpw,"\nFor more information about commands, use:\n");
	wprintw(helpw,"Usage: help [command[s]]\n");
	wprintw(helpw,"Usage: ? [command[s]]\n");
	break;
    }

    wprintw(helpw,"\nenter any key to exit help : ");
    wattron(helpw, A_NORMAL);
    wrefresh(helpw);

    c = wgetch(helpw);
    
    (void) delwin(helpw);
    (void) wclear(window->cwin);
    (void) wclear(inw);
    
    return (1);
}

/*
 * move function, relative movement of cursor.
 */
int
c_move(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long px, py, dpx, dpy;

#ifdef DEBUG
    fprintf(stdout,"move: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &dpx, &dpy) != 2) return (0);
    if (! cur_getcursor(window, &px, &py)) return (0);
    if (! cur_setcursor(window, (px + dpx), (py + dpy))) return (0);
    return (1);
}

/*
 * execute function.
 */
int
c_execute(window, inw, string, inlu)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
FILE **inlu;
{
    FILE *fopen();
    char *pstr, *pstrend;

#ifdef DEBUG
    fprintf(stdout,"execute: %s.\n",*string);
#endif

    pstr = *string;
    for ( ; *pstr == ' '; pstr++)
    {
	/* do nothing */
    }

    pstrend = pstr;
    for ( ; (*pstrend != ' ') && (*pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    *pstrend = '\0';

    if (*inlu != (FILE *) NULL) (void) fclose(*inlu);

    if (*pstr == '\0')
    {
	*inlu = (FILE *) NULL;
    }
    else if ((*inlu = fopen(pstr, "r")) == NULL) 
    {
	*inlu = (FILE *) NULL;	/* prevent lock out */
	return (0);
    }
    return (1);
}

/*
 * shell function.
 */
int
c_shell(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long rc;
    char *pstr;
    int c;

#ifdef DEBUG
    fprintf(stdout,"shell: %s.\n",*string);
#endif
    for (pstr = *string; *pstr == ' '; pstr++)
    {
	/* do nothing */
    }

    /* set terminal to shell mode */
    reset_shell_mode();

    /* assume success */
    rc = 1;

    if (system(pstr) < 0) rc = 0;

    /* let user decide when to continue */
    fprintf(stdout,"\nenter any key to continue: ");
    c = getc(stdin);

    /* set terminal to  program mode */
    reset_prog_mode();

    /* refresh screen completely */

    (void) wclear(window->cwin);
    (void) wclear(inw);

    return (rc);
}

/*
 * clear function.
 */
int
c_clear(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    /* refresh screen completely */

    (void) wclear(window->cwin);
    (void) wclear(inw);

    return (1);
}

/*
 * echo function.
 */
int
c_echo(window, inw, string, flag)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
long *flag;
{
    /*
     * commands at ECHO level.
     */
    long status, key;
   
    switch ((key = get_comm(comecho, 4L, string)))
    {
    case ECHO_YES:
	if (! (status = echo_yes(string, flag)))
	{
	    wprintw(inw,"ECHO: yes error.\n");
	}
	break;
    case ECHO_NO:
	if (! (status = echo_no(string, flag)))
	{
	    wprintw(inw,"ECHO: no error.\n");
	}
	break;
    case ECHO_WAIT:
	if (! (status = echo_wait(string, flag)))
	{
	    wprintw(inw,"ECHO: wait error.\n");
	}
	break;
    default:
	status = 0;
	wprintw(inw,"ECHO: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * display function.
 */
int
c_display(window, inw, string, flag)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
long *flag;
{
    /*
     * commands at DISPLAY level.
     */
    long status, key;
   
    switch ((key = get_comm(comdisp, 4L, string)))
    {
    case DISPLAY_NO:
	status = 1;
	*flag = DISPLAY_NO;
	break;
    case DISPLAY_YES:
    default:
	status = 1;
	*flag = DISPLAY_YES;
	break;
    }
    return (status);
}

/*
 * echo yes function.
 */
int
echo_yes(string, flag)
char *string[];
long *flag;
{
#ifdef DEBUG
    fprintf(stdout,"echo yes: %s.\n",*string);
#endif
    *flag = ECHO_YES;
    return (1);
}

/*
 * echo no function.
 */
int
echo_no(string, flag)
char *string[];
long *flag;
{
#ifdef DEBUG
    fprintf(stdout,"echo no: %s.\n",*string);
#endif
    *flag = ECHO_NO;
    return (1);
}

/*
 * echo wait function.
 */
int
echo_wait(string, flag)
char *string[];
long *flag;
{
#ifdef DEBUG
    fprintf(stdout,"echo wait: %s.\n",*string);
#endif
    *flag = ECHO_WAIT;
    return (1);
}

/*
 * arc function.
 */
int
c_arc(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    char *pstr;
    long px, py, level;
    double radius, alpha, aarc;

#ifdef DEBUG
    fprintf(stdout,"arc: %s.\n",*string);
#endif
    pstr = *string;
    if (sscanf(pstr,"%lf %lf %lf %d", 
	&radius, &alpha, &aarc, &level) != 4) return (0);
    if (! cur_getcursor(window, &px, &py)) return (0);
    if (! cur_arc(window, level, radius, aarc, 
	          (double) px, (double) py, alpha)) return (0);
    return (1);
}

/*
 * get gray picture from a file.
 */
int
c_filein(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    FILE *lu, *fopen();
    char *pstr, *pstrend;

#ifdef DEBUG
    fprintf(stdout,"file in: %s.\n",*string);
#endif
    pstr = *string;
    for ( ; *pstr == ' '; pstr++)
    {
	/* do nothing */
    }
    pstrend = pstr;
    for ( ; (*pstrend != ' ') && ( *pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    *pstrend = '\0';
    if ((lu = fopen(pstr, "r")) == NULL) return (0);
    /*
     * read picture and store
     */
    (void) cur_readscr(window, lu);
    (void) fclose(lu);
    return (1);
}

/*
 * put gray picture to a file.
 */
int
c_fileout(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    FILE *lu, *fopen();
    char *pstr, *pstrend;

#ifdef DEBUG
    fprintf(stdout,"file out: %s.\n",*string);
#endif
    pstr = *string;
    for ( ; *pstr == ' '; pstr++)
    {
	/* do nothing */
    }
    pstrend = pstr;
    for ( ; (*pstrend != ' ') && ( *pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    *pstrend = '\0';
    if ((lu = fopen(pstr, "w")) == NULL) return (0);
    /*
     * write picture
     */
    (void) cur_writescr(window, lu);
    (void) fclose(lu);
    return (1);
}

/*
 * change one gray level to another level function.
 */
int
c_change(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long oldlevel, newlevel;

#ifdef DEBUG
    fprintf(stdout,"change: %s.\n",*string);
#endif
    if (sscanf(*string,"%d %d", &oldlevel, &newlevel) != 2) return (0);
    if (! cur_chglvl(window, oldlevel, newlevel)) return (0);
    return (1);
}

/*
 * encode screen.
 */
int
c_encode(window, inw, string)
CUR_WINDOW *window;
WINDOW *inw;
char *string[];
{
    long state, x, y, oldx, oldy, oldg, level;
    char *pstr, *pstrend;
    FILE *lu, *fopen();

#ifdef DEBUG
    fprintf(stdout,"encode: %s.\n",*string);
#endif

    /*
     * open output file.
     */
    pstr = *string;
    for ( ; *pstr == ' '; pstr++)
    {
	/* do nothing */
    }
    pstrend = pstr;
    for ( ; (*pstrend != ' ') && ( *pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    *pstrend = '\0';
    if ((lu = fopen(pstr, "w")) == NULL) return (0);

    /* disable echoing, commands are executed faster */
    fprintf(lu, "echo no\n");
    fprintf(lu, "display no\n");

    /* store gray level colors */
    for (level = 0; level <= MAXCOLOR; level++)
    {
	fprintf(lu, "set foregound %d %c\n", 
	        level, window->palette[(int)level]);
    }

    /* start encoding picture */
    for (y = 0; y <= window->maxpy; y++)
    {
	/* one line at a time. */
	oldx = 0L;
	oldy = y;
	if ((window->gray[oldy][oldx] & 0200) == 0200)
	{
	    /* character state */
	    state = CHAR_S;
	}
	else
	{
	    /* gray level state */
	    oldg = window->gray[oldy][oldx];
	    state = GRAY_S;
	}

	for (x = 0; x <= window->maxpx; x++)
	{
	    switch (state)
	    {
	    case CHAR_S:
		if ((window->gray[y][x] & 0200) == 0200)
		{
		    /* stay in the same state */
		    state = CHAR_S;
		    continue;
		}

		/* gray level found, write out string */
		fprintf(lu, "set cursor %d %d\n", oldx, oldy);
		fprintf(lu, "put string 2 ");
		for (pstr = (char *)&window->gray[oldy][oldx];
		     pstr < (char *)&window->gray[y][x];
		     pstr++)
		{
		    putc((*pstr & 0177), lu);
		}
		fprintf(lu, "\n");

		/* switch to gray level state */
		oldx = x;
		oldy = y;
		oldg = window->gray[y][x];
		state = GRAY_S;
		break;

	    case GRAY_S:
		if ((window->gray[y][x] & 0200) != 0200)
		{
		    /* still a gray level */
		    if (window->gray[y][x] == oldg)
		    {
			/* same gray level, continue */
			state = GRAY_S;
			continue;
		    }

		    /* different gray level, write line */
		    fprintf(lu, "set cursor %d %d\n", oldx, oldy);
		    fprintf(lu, "line absolute %d %d %d\n",
			    (x - 1), y, oldg);

		    /* new gray level state */
		    oldx = x;
		    oldy = y;
	  	    oldg = window->gray[y][x];
		    state = GRAY_S;
		    continue;
	   	}

	 	/* character found, draw gray level line */
		fprintf(lu, "set cursor %d %d\n", oldx, oldy);
		fprintf(lu, "line absolute %d %d %d\n",
			(x - 1), y, oldg);

		/* switch to character state */
		oldx = x;
		oldy = y;
		state = CHAR_S;
		break;

	    default:
		/* error, bad state */
		(void) fclose(lu);
		return (0);
	    }
	}

	switch (state)
	{
	case CHAR_S:
	    /* write remaining characters */
	    fprintf(lu, "set cursor %d %d\n", oldx, oldy);
	    fprintf(lu, "put string 2 ");
	    for (pstr = (char *)&window->gray[oldy][oldx];
		 pstr < (char *)&window->gray[y][x];
		 pstr++)
	    {
		putc((*pstr & 0177), lu);
	    }
	    fprintf(lu, "\n");
	    break;

	case GRAY_S:
	    /* write out last part of line */
	    fprintf(lu, "set cursor %d %d\n", oldx, oldy);
	    fprintf(lu, "line absolute %d %d %d\n",
		    (x - 1), y, oldg);
	    break;
	}
    }

    fprintf(lu, "display yes\n");
    fprintf(lu, "quit\n");

    /* close file and return */
    (void) fclose(lu);
    return (1);
}
/*
 * main driver.
 */
main(argc, argv)
int argc;
char *argv[];
{
    /*
     * file logical unit.
     */
    FILE *inlu;

    /*
     * window structures.
     */
    CUR_WINDOW *outw;	/* gray level window (only for output) */
    WINDOW *inw;	/* curses window, input and output */

    /*
     * locations and sizes of windows.
     */
    long inx0, iny0, innx, inny;
    long outx0, outy0, outnx, outny;

    /*
     * functions.
     */
    long cur_getwindow();
    void good_bye(), cur_initscr();

    /*
     * other junk.
     */
    char *cptr, input[100];
    long outsig, flag, key, status, echo_flag, disp_flag;

    outsig = 1;			/* exit status, 1=failure, 0=success */
    inlu = (FILE *) NULL;	/* null pointer */
    echo_flag = ECHO_NO;
    disp_flag = DISPLAY_YES;

    /*
     * initialize curses.
     */
    cur_initscr();

    /*
     * define interactive window, and gray level output window.
     */
    inx0 = 0;
    iny0 = 0;
    innx = COLS;
    inny = LINES - 4;
    outx0 = 0;
    outy0 = LINES - 4;
    outnx = COLS;
    outny = 4;
    if (! cur_getwindow(&outw, inx0, iny0, innx, inny))
    {
	good_bye(outsig, stderr, "edcur: unable to define CUR_WINDOW.");
    }
    if ((inw = newwin((int) outny, (int) outnx, 
		      (int) outy0, (int) outx0)) == NULL)
    {
	good_bye(outsig, stderr, "edcur: unable to define WINDOW.");
    }

    /*
     * set initial attributes for windows and physical screen.
     */
    cbreak();			/* no input buffering */
    echo();			/* echo of input */
    idlok(inw, TRUE);		/* enable hardware scrolling */
    scrollok(inw, TRUE);	/* enable scrolling */
    wsetscrreg(inw, 0, 3);	/* where to scroll */

    /*
     * main input from user. read and execute.
     */
    flag = 1;
    status = 0;
    (void) cur_setcursor(outw, 0L, 0L);
    wmove(inw, 0, 0);		/* set cursor in input window */
    while ( flag )
    {
	/*
	 * get commands from user.
	 */
	if (inlu == (FILE *) NULL)
	{
	    wprintw(inw,"input> ");
	    if (wgetstr(inw, input) != OK)
	    {
		*input = '\0';		/* ignore line */
	    }
	}
	else if (char_gets(input, 100, inlu) == NULL)
	{
	    wprintw(inw, "EDCUR: EOF or error reading alternate input.\n");
	    (void) fclose(inlu);
	    inlu = (FILE *) NULL;
	    *input = '\0';
	}

	/*
	 * echo input.
	 */
	switch (echo_flag)
	{
	case ECHO_WAIT:
	    (void) sleep(1);
	case ECHO_YES:
	    wprintw(inw,"%s\n",input);
	    break;
	default:
	    break;
	}

	/*
	 * parse command and execute.
	 */
	cptr = input;	/* point to next token in string */

	switch ((key = get_comm(comlev1, 4L, &cptr)))
	{
	case L1_SET:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_set(outw, inw, &cptr);
	    break;
	case L1_GET:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_get(outw, inw, &cptr);
	    break;
	case L1_PUT:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_put(outw, inw, &cptr);
	    break;
	case L1_LINE:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_line(outw, inw, &cptr);
	    break;
	case L1_WINDOW:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_window(outw, inw, &cptr);
	    break;
	case L1_ERASE:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_erase(outw, inw, &cptr);
	    break;
	case L1_FILL:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_fill(outw, inw, &cptr);
	    break;
	case L1_QUIT:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    if (inlu == (FILE *) NULL)
	    {
		status = 1;
		flag = 0;		/* exit program */
	    }
	    else
	    {
		wprintw(inw, "Closing alternate input.\n");
		(void) fclose(inlu);
		inlu = (FILE *) NULL;
	    }
	    break;
	case L1_HELP:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_help(outw, inw, &cptr);
	    break;
	case L1_MOVE:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_move(outw, inw, &cptr);
	    break;
	case L1_EXECUTE:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_execute(outw, inw, &cptr, &inlu);
	    break;
	case L1_SHELL:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_shell(outw, inw, &cptr);
	    break;
	case L1_COMMENT:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = 1;
	    break;
	case L1_ECHO:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_echo(outw, inw, &cptr, &echo_flag);
	    break;
	case L1_DISPLAY:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_display(outw, inw, &cptr, &disp_flag);
	    break;
	case L1_FILEIN:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_filein(outw, inw, &cptr);
	    break;
	case L1_FILEOUT:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_fileout(outw, inw, &cptr);
	    break;
	case L1_ARC:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_arc(outw, inw, &cptr);
	    break;
	case L1_CHANGE:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_change(outw, inw, &cptr);
	    break;
	case L1_ENCODE:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_encode(outw, inw, &cptr);
	    break;
	case L1_CLEAR:
#ifdef DEBUG
	    wprintw(inw, "Command is %s.\n", comlev1[key].com);
#endif
	    status = c_clear(outw, inw, &cptr);
	    break;
	case TOOMANY:
	    if (inlu == (FILE *) NULL)
	    {
		wprintw(inw,"Too many matches, try again.\n");
	    }
	    else
	    {
		wprintw(inw,"EDCUR: error in alternate input.\n");
		wprintw(inw,"Too many matches, try again.\n");
		wprintw(inw,"closing alternate input.\n");
		(void) fclose(inlu);
		inlu = (FILE *) NULL;
	    }
	    status = 1;
	    break;
	case BLANKLINE:
	    status = 1;
	    break;
	case NOMATCH:
	default:
	    if (inlu == (FILE *) NULL)
	    {
		wprintw(inw,"No match, try again.\n");
	    }
	    else
	    {
		wprintw(inw,"EDCUR: error in alternate input.\n");
		wprintw(inw,"No match, try again.\n");
		wprintw(inw,"closing alternate input.\n");
		(void) fclose(inlu);
		inlu = (FILE *) NULL;
	    }
	    status = 1;
	    break;
	}

	/*
	 * error in identified command.
	 */
	if (! status)
	{
	    wprintw(inw,"EDCUR: error in command: %s\n", input);
	    wprintw(inw,"Try again.\n");
	    if (inlu != (FILE *) NULL)
	    {
		(void) fclose(inlu);
		inlu = (FILE *) NULL;
	    }
	}

	if (disp_flag == DISPLAY_YES)
	{
	    /*
	     * refresh both windows.
	     */
	    (void) cur_write(outw, 0L);
	    wrefresh(inw);
	    /*
	     * display cursor, but skip Gray level matrix.
	     */
	    wmove(outw->cwin, (int) outw->py, (int) outw->px);
	    waddch(outw->cwin, 'C');
	    wrefresh(outw->cwin);
	}
    }

    /*
     * exit program.
     */
    good_bye(0L, stdout, "EDCUR exit.");
}
