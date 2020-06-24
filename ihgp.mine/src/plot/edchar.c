/*
 * this file contains the code for a character plotting editor.
 */

/*
 * standard headers.
 */
#include <string.h>
#include <stdio.h>
#include <math.h>

/*
 * other headers.
 */
#include "charconv.h"
#include "charplot.h"

/*
 * local values.
 */
#define X_DEFAULT 75L
#define Y_DEFAULT 24L
#define BACK_DEFAULT "."

/*
 * commands structure.
 */
struct COMMANDS 
{
    char com[15];
    long key;
};

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
#define L1_SET 0L
#define L1_GET 1L
#define L1_PUT 2L
#define L1_LINE 3L
#define L1_WINDOW 4L
#define L1_ERASE 5L
#define L1_DISPLAY 6L
#define L1_FILL 7L
#define L1_QUIT 8L
#define L1_HELP 9L
#define L1_MOVE 10L
#define L1_FILE 11L
#define L1_EXECUTE 12L
#define L1_SHELL 13L
#define L1_COMMENT 14L
#define L1_ECHO 15L
#define L1_ARC 16L
#define L1_SCALE 17L

static struct COMMANDS comlev1[] = 
{
    {"SET", L1_SET},
    {"GET", L1_GET},
    {"PUT", L1_PUT},
    {"LINE", L1_LINE},
    {"WINDOW", L1_WINDOW},
    {"ERASE", L1_ERASE},
    {"DISPLAY", L1_DISPLAY},
    {"FILL", L1_FILL},
    {"QUIT", L1_QUIT},
    {"HELP", L1_HELP},
    {"MOVE", L1_MOVE},
    {"FILE", L1_FILE},
    {"EXECUTE", L1_EXECUTE},
    {"SHELL", L1_SHELL},
    {"#", L1_COMMENT},
    {"ECHO", L1_ECHO},
    {"ARC", L1_ARC},
    {"SCALE", L1_SCALE},
    {"?", L1_HELP},		/* duplicate commands at end */
    {"!", L1_SHELL},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at SET level.
 */
#define SET_SCREEN 0L
#define SET_BACKGROUND 1L
#define SET_FOREGROUND 2L
#define SET_CURSOR 3L

static struct COMMANDS comset[] = 
{
    {"SCREEN", SET_SCREEN},
    {"BACKGROUND", SET_BACKGROUND},
    {"FOREGROUND", SET_FOREGROUND},
    {"CURSOR", SET_CURSOR},
    {LASTCOM, LASTCOMKEY}
};

/*
 * commands at GET level.
 */
#define GET_SCREEN 0L
#define GET_BACKGROUND 1L
#define GET_FOREGROUND 2L
#define GET_CURSOR 3L
#define GET_COLOR 4L

static struct COMMANDS comget[] = 
{
    {"SCREEN", GET_SCREEN},
    {"BACKGROUND", GET_BACKGROUND},
    {"FOREGROUND", GET_FOREGROUND},
    {"CURSOR", GET_CURSOR},
    {"COLOR", GET_COLOR},
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
 * commands at DISPLAY level.
 */
#define DISPLAY_CURSOR 0L
#define DISPLAY_SCREEN 1L

static struct COMMANDS comdisplay[] = 
{
    {"CURSOR", DISPLAY_CURSOR},
    {"SCREEN", DISPLAY_SCREEN},
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
 * commands at FILE level.
 */
#define FILE_IN 0L
#define FILE_OUT 1L

static struct COMMANDS comfile[] = 
{
    {"IN", FILE_IN},
    {"OUT", FILE_OUT},
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
 * commands at SCALE level.
 */
#define SCALE_NO 0L
#define SCALE_YES 1L

static struct COMMANDS comscale[] = 
{
    {"NO", SCALE_NO},
    {"YES", SCALE_YES},
    {LASTCOM, LASTCOMKEY}
};

/*
 * get input string.
 */
char *char_gets(input, nchar, stream)
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
long get_comm(commands, nchar, string)
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
long set(string)
char *string[];
{
    /*
     * commands at SET level.
     */
    long status, key, get_comm();
   
    switch ((key = get_comm(comset, 4L, string)))
    {
    case SET_SCREEN:
	if (! (status = set_screen(string)))
	{
	    fprintf(stderr,"SET: screen error.\n");
	}
	break;
    case SET_BACKGROUND:
	if (! (status = set_background(string)))
	{
	    fprintf(stderr,"SET: background error.\n");
	}
	break;
    case SET_FOREGROUND:
	if (! (status = set_foreground(string)))
	{
	    fprintf(stderr,"SET: foreground error.\n");
	}
	break;
    case SET_CURSOR:
	if (! (status = set_cursor(string)))
	{
	    fprintf(stderr,"SET: cursor error.\n");
	}
	break;
    default:
	status = 0;
	fprintf(stderr,"SET: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * get function.
 */
long get(string)
char *string[];
{
    /*
     * commands at GET level.
     */
    long status, key, get_comm();

    switch ((key = get_comm(comget, 4L, string)))
    {
    case GET_SCREEN:
	if (! (status = get_screen(string)))
	{
	    fprintf(stderr,"GET: screen error.\n");
	}
	break;
    case GET_BACKGROUND:
	if (! (status = get_background(string)))
	{
	    fprintf(stderr,"GET: background error.\n");
	}
	break;
    case GET_FOREGROUND:
	if (! (status = get_foreground(string)))
	{
	    fprintf(stderr,"GET: foreground error.\n");
	}
	break;
    case GET_CURSOR:
	if (! (status = get_cursor(string)))
	{
	    fprintf(stderr,"GET: cursor error.\n");
	}
	break;
    case GET_COLOR:
	if (! (status = get_color(string)))
	{
	    fprintf(stderr,"GET: color error.\n");
	}
	break;
    default:
	status = 0;
	fprintf(stderr,"GET: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * put function.
 */
long put(string)
char *string[];
{
    /*
     * commands at PUT level.
     */
    long status, key, get_comm();

    switch ((key = get_comm(comput, 4L, string)))
    {
    case PUT_SYMBOL:
	if (! (status = put_symbol(string)))
	{
	    fprintf(stderr,"PUT: symbol error.\n");
	}
	break;
    case PUT_POINT:
	if (! (status = put_point(string)))
	{
	    fprintf(stderr,"PUT: point error.\n");
	}
	break;
    case PUT_STRING:
	if (! (status = put_string(string)))
	{
	    fprintf(stderr,"PUT: string error.\n");
	}
	break;
    default:
	status = 0;
	fprintf(stderr,"PUT: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * set screen function.
 */
long set_screen(string)
char *string[];
{
    long maxpx, maxpy;

#ifdef DEBUG
    fprintf(stdout,"set screen: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &maxpx, &maxpy) != 2) return (0);
    if (! char_setscr(maxpx, maxpy)) return (0);
    return (1);
}

/*
 * set background function.
 */
long set_background(string)
char *string[];
{
    char color;
    short nchar;

#ifdef DEBUG
    fprintf(stdout,"set background: %s.\n",*string);
#endif
/*
    if (sscanf(*string, "%1s", &color) != 1) return (0);
    if (! char_setback(&color)) return (0);
 */
    nchar = sscanf(*string, "%1s", &color);
    switch (nchar)
    {
    case 1:
	break;
    default:
	color = ' ';
	break;
    }
    if (! char_setback(&color)) return (0);
    return (1);
}

/*
 * set foreground function.
 */
long set_foreground(string)
char *string[];
{
    char color;
    short nchar;
    long level;

#ifdef DEBUG
    fprintf(stdout,"set foreground: %s.\n",*string);
#endif
/*
    if (sscanf(*string, "%d %1s", &level, &color) != 2) return (0);
    if (! char_setfore(&color, level)) return (0);
 */
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
    if (! char_setfore(&color, level)) return (0);
    return (1);
}

/*
 * set cursor function.
 */
long set_cursor(string)
char *string[];
{
    long px, py;

#ifdef DEBUG
    fprintf(stdout,"set cursor: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &px, &py) != 2) return (0);
    if (! char_setcursor(px, py)) return (0);
    return (1);
}

/*
 * get screen function.
 */
long get_screen(string)
char *string[];
{
    long maxpx, maxpy;

#ifdef DEBUG
    fprintf(stdout,"get screen: %s.\n",*string);
#endif
    if (! char_getscr(&maxpx, &maxpy)) return (0);
    fprintf(stdout,"MAXPX = %d, MAXPY = %d\n", maxpx, maxpy);
    return (1);
}

/*
 * get background function.
 */
long get_background(string)
char *string[];
{
    char color;

#ifdef DEBUG
    fprintf(stdout,"get background: %s.\n",*string);
#endif
    if (! char_getback(&color)) return (0);
    fprintf(stdout,"background color: %c\n", color);
    return (1);
}

/*
 * get foreground function.
 */
long get_foreground(string)
char *string[];
{
    char color;
    long level;

#ifdef DEBUG
    fprintf(stdout,"get foreground: %s.\n",*string);
#endif
    if (sscanf(*string, "%d", &level) != 1) return (0);
    if (! char_getfore(&color, level)) return (0);
    fprintf(stdout,"foreground color for level %d: %c\n", level, color);
    return (1);
}

/*
 * get cursor function.
 */
long get_cursor(string)
char *string[];
{
    long px, py;

#ifdef DEBUG
    fprintf(stdout,"get cursor: %s.\n",*string);
#endif
    if (! char_getcursor(&px, &py)) return (0);
    fprintf(stdout,"cursor position (x,y) = (%d,%d)\n", px, py);
    return (1);
}

/*
 * get color function (return ASCII or gray level).
 */
long get_color(string)
char *string[];
{
    long px, py;
    long level;
    char ascii;

#ifdef DEBUG
    fprintf(stdout,"get color: %s.\n",*string);
#endif
    if (! char_getcursor(&px, &py)) return (0);
    switch (char_pointcolor(px, py, &level, &ascii))
    {
    case ASCII:
	fprintf(stdout, "character at (%d,%d): %c\n", px, py, ascii);
	break;
    case GRAY:
	fprintf(stdout, "gray level at (%d,%d): %d\n", px, py, level);
	break;
    default:
	return (0);
    }
    return (1);
}

/*
 * put symbol function.
 */
long put_symbol(string)
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
    if (! char_putsym(pstr)) return (0);
    return (1);
}

/*
 * put point function.
 */
long put_point(string)
char *string[];
{
    long level;

#ifdef DEBUG
    fprintf(stdout,"put point: %s.\n",*string);
#endif
    if (sscanf(*string, "%d", &level) != 1) return (0);
    if (! char_putpoint(level)) return (0);
    return (1);
}

/*
 * put string function.
 */
long put_string(string)
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
    if (! char_putstring(pstr, flag)) return (0);
    return (1);
}

/*
 * line function.
 */
long line(string)
char *string[];
{
    /*
     * commands at LINE level.
     */
    long key, get_comm();

    switch ((key = get_comm(comline, 4L, string)))
    {
    case LINE_RELATIVE:
	if (! line_relative(string))
	{
	    fprintf(stderr,"LINE: relative error.\n");
	}
	break;
    case LINE_ABSOLUTE:
    default:
	if (! line_absolute(string))
	{
	    fprintf(stderr,"LINE: absolute error.\n");
	}
	break;
    }
    return (1);
}

/*
 * line absolute function.
 */
long line_absolute(string)
char *string[];
{
    long px, py, level;

#ifdef DEBUG
    fprintf(stdout,"line absolute: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d %d", &px, &py, &level) != 3) return (0);
    if (! char_putline(px, py, level)) return (0);
    return (1);
}

/*
 * line relative function.
 */
long line_relative(string)
char *string[];
{
    long dpx, dpy, px, py, level;

#ifdef DEBUG
    fprintf(stdout,"line relative: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d %d", &dpx, &dpy, &level) != 3) return (0);
    if (! char_getcursor(&px, &py)) return (0);
    if (! char_putline((px + dpx), (py + dpy), level)) return (0);
    return (1);
}

/*
 * window function.
 */
long window(string)
char *string[];
{
    long px0, py0, px1, py1, level;

#ifdef DEBUG
    fprintf(stdout,"window: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d %d %d %d", 
	       &px0, &py0, &px1, &py1, &level) != 5) return (0);
    if (! char_paintw(px0, py0, px1, py1, level)) return (0);
    return (1);
}

/*
 * erase function.
 */
long erase(string)
char *string[];
{
#ifdef DEBUG
    fprintf(stdout,"erase: %s.\n",*string);
#endif
    if (! char_erase()) return (0);
    return (1);
}

/*
 * display function.
 */
long display(string, outlu, scale)
char *string[];
FILE *outlu;
long scale;
{
    /*
     * commands at DISPLAY level.
     */
    long key, get_comm();

    switch ((key = get_comm(comdisplay, 4L, string)))
    {
    case DISPLAY_CURSOR:
	if (! display_cursor(string, outlu, scale))
	{
	    fprintf(stderr,"DISPLAY: cursor error.\n");
	}
	break;
    case DISPLAY_SCREEN:
    default:
	if (! display_screen(string, outlu, scale))
	{
	    fprintf(stderr,"DISPLAY: screen error.\n");
	}
	break;
    }
    return (1);
}

/*
 * display cursor function.
 */
long display_cursor(string, outlu, scale)
char *string[];
FILE *outlu;
long scale;
{
    long px, py;
    long level;
    unsigned char ascii;

#ifdef DEBUG
    fprintf(stdout,"display cursor: %s.\n",*string);
#endif
    if (! char_getcursor(&px, &py)) return (0);
    switch (char_pointcolor(px, py, &level, &ascii))
    {
    case ASCII:
	(void) char_putsym("C");
	(void) char_writescr(outlu, scale);
	(void) char_putsym(&ascii);
	break;
    case GRAY:
	(void) char_putsym("C");
	(void) char_writescr(outlu, scale);
	(void) char_putpoint(level);
	break;
    default:
	return (0);
    }
    return (1);
}

/*
 * display screen function.
 */
long display_screen(string, outlu, scale)
char *string[];
FILE *outlu;
long scale;
{
#ifdef DEBUG
    fprintf(stdout,"display screen: %s.\n",*string);
#endif
    if (! char_writescr(outlu, scale)) return (0);
    return (1);
}

/*
 * fill function, assume over write ASCII.
 */
long fill(string)
char *string[];
{
    long px, py, level, contour;

#ifdef DEBUG
    fprintf(stdout,"fill: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &level, &contour) != 2) return (0);
    if (! char_getcursor(&px, &py)) return (0);
    if (! char_fill(px, py, level, contour, OWASCII)) return (0);
    return (1);
}

/*
 * help function.
 */
long help(string)
char *string[];
{
    long key, get_comm();

    switch ((key = get_comm(comlev1, 4L, string)))
    {
    case L1_SET:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comset, 4L, string)))
	{
	case SET_SCREEN:
	    fprintf(stdout,"Usage: SET SCREEN maxpx maxpy\n");
	    break;
	case SET_BACKGROUND:
	    fprintf(stdout,"Usage: SET BACKGROUND color\n");
	    break;
	case SET_FOREGROUND:
	    fprintf(stdout,"Usage: SET FOREGROUND level color\n");
	    break;
	case SET_CURSOR:
	    fprintf(stdout,"Usage: SET CURSOR px py\n");
	    break;
	default:
	    fprintf(stdout,"Usage: SET SCREEN maxpx maxpy\n");
	    fprintf(stdout,"Usage: SET BACKGROUND color\n");
	    fprintf(stdout,"Usage: SET FOREGROUND level color\n");
	    fprintf(stdout,"Usage: SET CURSOR px py\n");
	    break;
	}
	break;
    case L1_GET:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comget, 4L, string)))
	{
	case GET_SCREEN:
	    fprintf(stdout,"Usage: GET SCREEN\n");
	    break;
	case GET_BACKGROUND:
	    fprintf(stdout,"Usage: GET BACKGROUND\n");
	    break;
	case GET_FOREGROUND:
	    fprintf(stdout,"Usage: GET FOREGROUND level\n");
	    break;
	case GET_CURSOR:
	    fprintf(stdout,"Usage: GET CURSOR\n");
	    break;
	case GET_COLOR:
	    fprintf(stdout,"Usage: GET COLOR\n");
	    break;
	default:
	    fprintf(stdout,"Usage: GET SCREEN\n");
	    fprintf(stdout,"Usage: GET BACKGROUND\n");
	    fprintf(stdout,"Usage: GET FOREGROUND level\n");
	    fprintf(stdout,"Usage: GET CURSOR\n");
	    fprintf(stdout,"Usage: GET COLOR\n");
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
	    fprintf(stdout,"Usage: PUT SYMBOL character\n");
	    break;
	case PUT_POINT:
	    fprintf(stdout,"Usage: PUT POINT level\n");
	    break;
	case PUT_STRING:
	    fprintf(stdout,"Usage: PUT STRING direction character_string\n");
	    break;
	default:
	    fprintf(stdout,"Usage: PUT SYMBOL character\n");
	    fprintf(stdout,"Usage: PUT POINT level\n");
	    fprintf(stdout,"Usage: PUT STRING direction character_string\n");
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
	    fprintf(stdout,"Usage: LINE RELATIVE dpx dpy level\n");
	    break;
	case LINE_ABSOLUTE:
	    fprintf(stdout,"Usage: LINE [ABSOLUTE] px py level\n");
	    break;
	default:
	    fprintf(stdout,"Usage: LINE RELATIVE dpx dpy level\n");
	    fprintf(stdout,"Usage: LINE [ABSOLUTE] px py level\n");
	    break;
	}
	break;
    case L1_WINDOW:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: WINDOW px0 py0 px1 py1 level\n");
	break;
    case L1_ERASE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: ERASE\n");
	break;
    case L1_DISPLAY:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comdisplay, 4L, string)))
	{
	case DISPLAY_CURSOR:
	    fprintf(stdout,"Usage: DISPLAY CURSOR\n");
	    break;
	case DISPLAY_SCREEN:
	    fprintf(stdout,"Usage: DISPLAY [SCREEN]\n");
	    break;
	default:
	    fprintf(stdout,"Usage: DISPLAY CURSOR\n");
	    fprintf(stdout,"Usage: DISPLAY [SCREEN]\n");
	    break;
	}
	break;
    case L1_FILL:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: FILL fill_level contour_level\n");
	break;
    case L1_QUIT:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: QUIT\n");
	break;
    case L1_HELP:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: HELP [command[s]]\n");
	fprintf(stdout,"Usage: ? [command[s]]\n");
	break;
    case L1_MOVE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: MOVE dpx dpy\n");
	break;
    case L1_FILE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comfile, 4L, string)))
	{
	case FILE_IN:
	    fprintf(stdout,"Usage: FILE IN input_file\n");
	    break;
	case FILE_OUT:
	    fprintf(stdout,"Usage: FILE OUT [output_file]\n");
	    break;
	default:
	    fprintf(stdout,"Usage: FILE IN input_file\n");
	    fprintf(stdout,"Usage: FILE OUT [output_file]\n");
	    break;
	}
	break;
    case L1_EXECUTE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: EXEC commands_file\n");
	break;
    case L1_SHELL:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: SHELL command\n");
	fprintf(stdout,"Usage: ! command\n");
	break;
    case L1_COMMENT:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: # [comment[s]]\n");
	break;
    case L1_ECHO:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comecho, 4L, string)))
	{
	case ECHO_YES:
	    fprintf(stdout,"Usage: ECHO YES\n");
	    break;
	case ECHO_NO:
	    fprintf(stdout,"Usage: ECHO NO\n");
	    break;
	case ECHO_WAIT:
	    fprintf(stdout,"Usage: ECHO WAIT\n");
	    break;
	default:
	    fprintf(stdout,"Usage: ECHO YES\n");
	    fprintf(stdout,"Usage: ECHO NO\n");
	    fprintf(stdout,"Usage: ECHO WAIT\n");
	    break;
	}
	break;
    case L1_SCALE:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	switch ((key = get_comm(comscale, 4L, string)))
	{
	case SCALE_YES:
	    fprintf(stdout,"Usage: SCALE YES\n");
	    break;
	case SCALE_NO:
	    fprintf(stdout,"Usage: SCALE NO\n");
	    break;
	default:
	    fprintf(stdout,"Usage: SCALE YES\n");
	    fprintf(stdout,"Usage: SCALE NO\n");
	    break;
	}
	break;
    case L1_ARC:
#ifdef DEBUG
	fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	fprintf(stdout,"Usage: ARC radius alpha arc level\n");
	break;
    default:
	fprintf(stdout,"List of commands\n");
	fprintf(stdout,"----------------\n");
	for (key = 0; comlev1[key].com[0] != LASTCOM; key++)
	{
	    fprintf(stdout,"%s ",comlev1[key].com);
	    if (((key % 3) == 0) && (key != 0)) fprintf(stdout,"\n");
	}
	fprintf(stdout,"\n");
	fprintf(stdout,"\nFor more information about commands, use:\n");
	fprintf(stdout,"Usage: help [command[s]]\n");
	fprintf(stdout,"Usage: ? [command[s]]\n");
	break;
    }
    return (1);
}

/*
 * move function, relative movement of cursor.
 */
long move(string)
char *string[];
{
    long px, py, dpx, dpy;

#ifdef DEBUG
    fprintf(stdout,"move: %s.\n",*string);
#endif
    if (sscanf(*string, "%d %d", &dpx, &dpy) != 2) return (0);
    if (! char_getcursor(&px, &py)) return (0);
    if (! char_setcursor((px + dpx), (py + dpy))) return (0);
    return (1);
}

/*
 * file function.
 */
long file(string, outlu, scale)
char *string[];
FILE **outlu;
long scale;
{
    /*
     * commands at FILE level.
     */
    long status, key, get_comm();
   
    switch ((key = get_comm(comfile, 4L, string)))
    {
    case FILE_IN:
	if (! (status = file_in(string, scale)))
	{
	    fprintf(stderr,"FILE: in error.\n");
	}
	break;
    case FILE_OUT:
	if (! (status = file_out(string, outlu)))
	{
	    fprintf(stderr,"FILE: out error.\n");
	}
	break;
    default:
	status = 0;
	fprintf(stderr,"FILE: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * file in function.
 */
long file_in(string, scale)
char *string[];
{
    FILE *inlu, *fopen();
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
    for ( ; (*pstrend != ' ') && (*pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    *pstrend = '\0';
    if ((inlu = fopen(pstr, "r")) == NULL) 
    {
	return (0);
    }
    /*
     * read picture in and store in matrix.
     */
    (void) char_readscr(inlu, scale);
    (void) fclose(inlu);
    return (1);
}

/*
 * file out function.
 */
long file_out(string, outlu)
char *string[];
FILE **outlu;
{
    FILE *fopen();
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
    for ( ; (*pstrend != ' ') && (*pstrend != '\0'); pstrend++)
    {
	/* do nothing */
    }
    *pstrend = '\0';

    if (*outlu != stdout) 
    {
	(void) fclose(*outlu);
    }

    if (*pstr == '\0')
    {
	*outlu = stdout;
    }
    else if ((*outlu = fopen(pstr, "w")) == NULL) 
    {
	*outlu = stdout;		/* prevent lock out */
	return (0);
    }
    return (1);
}

/*
 * execute function.
 */
long execute(string, inlu)
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

    if (*inlu != stdin) (void) fclose(*inlu);

    if (*pstr == '\0')
    {
	*inlu = stdin;
    }
    else if ((*inlu = fopen(pstr, "r")) == NULL) 
    {
	*inlu = stdin;		/* prevent lock out */
	return (0);
    }
    return (1);
}

/*
 * shell function.
 */
long shell(string)
char *string[];
{
    char *pstr;

#ifdef DEBUG
    fprintf(stdout,"shell: %s.\n",*string);
#endif
    for (pstr = *string; *pstr == ' '; pstr++)
    {
	/* do nothing */
    }
    if (system(pstr) < 0) return (0);
    return (1);
}

/*
 * echo function.
 */
long echo(string, flag)
char *string[];
long *flag;
{
    /*
     * commands at ECHO level.
     */
    long status, key, get_comm();
   
    switch ((key = get_comm(comecho, 4L, string)))
    {
    case ECHO_YES:
	if (! (status = echo_yes(string, flag)))
	{
	    fprintf(stderr,"ECHO: yes error.\n");
	}
	break;
    case ECHO_NO:
	if (! (status = echo_no(string, flag)))
	{
	    fprintf(stderr,"ECHO: no error.\n");
	}
	break;
    case ECHO_WAIT:
	if (! (status = echo_wait(string, flag)))
	{
	    fprintf(stderr,"ECHO: wait error.\n");
	}
	break;
    default:
	status = 0;
	fprintf(stderr,"ECHO: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * echo yes function.
 */
long echo_yes(string, flag)
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
long echo_no(string, flag)
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
long echo_wait(string, flag)
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
 * scale function.
 */
long scale(string, flag)
char *string[];
long *flag;
{
    /*
     * commands at SCALE level.
     */
    long status, key, get_comm();
   
    switch ((key = get_comm(comscale, 4L, string)))
    {
    case SCALE_YES:
	if (! (status = scale_yes(string, flag)))
	{
	    fprintf(stderr,"SCALE: yes error.\n");
	}
	break;
    case SCALE_NO:
	if (! (status = scale_no(string, flag)))
	{
	    fprintf(stderr,"SCALE: no error.\n");
	}
	break;
    default:
	status = 0;
	fprintf(stderr,"SCALE: error, try again.\n");
	break;
    }
    return (status);
}

/*
 * scale yes function.
 */
long scale_yes(string, flag)
char *string[];
long *flag;
{
#ifdef DEBUG
    fprintf(stdout,"scale yes: %s.\n",*string);
#endif
    *flag = SCALE_YES;
    return (1);
}

/*
 * scale no function.
 */
long scale_no(string, flag)
char *string[];
long *flag;
{
#ifdef DEBUG
    fprintf(stdout,"scale no: %s.\n",*string);
#endif
    *flag = SCALE_NO;
    return (1);
}

/*
 * arc function.
 */
long arc(string)
char *string[];
{
    char *pstr;
    long px, py, level;
    double radius, alpha, aarc;

#ifdef DEBUG
    fprintf(stdout,"arc: %s.\n",*string);
#endif
    pstr = *string;
    if (sscanf(pstr,"%f %f %f %d", 
	&radius, &alpha, &aarc, &level) != 4) return (0);
    if (! char_getcursor(&px, &py)) return (0);
    if (! char_arc(level, radius, aarc, 
	           (double) px, (double) py, alpha)) return (0);
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
     * commands at upper level.
     */
    long key, get_comm();
    /*
     * other junk.
     */
    long scale_flag, echo_flag, flag, status;
    char input[100];
    char *cptr, *char_gets();
    /*
     * files for I/O.
     */
    int c;
    FILE *inlu, *alt_inlu, *outlu, *fopen();
    char *infile, *outfile;
    extern char *optarg;
    extern int optind;

    scale_flag = SCALE_NO;	/* don't scale x-axis */
    echo_flag = ECHO_NO;	/* don't echo commands */
    infile = NULLADDR;
    outfile = NULLADDR;
    inlu = stdin;
    outlu = stdout;
    alt_inlu = stdin;	/* alternate lu for files */
    /*
     * parse the command line.
     */
    while ((c = getopt(argc, argv, "i:o:")) != EOF)
    {
	switch (c)
	{
	case 'i':
	    infile = optarg;	/* input file name */
	    break;
	case 'o':
	    outfile = optarg;	/* output file name */
	    break;
	default:
	    fprintf(stderr,"EDCHAR: error in command line.\n");
	    fprintf(stderr,"Usage: edchar [-i infile] [-o outfile].\n");
	    exit (4);
	}
    }

    /*
     * open I/O files.
     */
    if (infile != NULLADDR)
    {
	if ((inlu = fopen(infile, "r")) == NULL)
	{
	    fprintf(stderr,"EDCHAR: unable to open file %s.\n", infile);
	    exit (4);
	}
    }
    if (outfile != NULLADDR)
    {
	if ((outlu = fopen(outfile, "w")) == NULL)
	{
	    fprintf(stderr,"EDCHAR: unable to open file %s.\n", infile);
	    exit (4);
	}
    }
    /*
     * set default screen size.
     */
    (void) char_setscr(X_DEFAULT, Y_DEFAULT);
    (void) char_setback(BACK_DEFAULT);
    /*
     * main input.
     */
    flag = 1;
    while ( flag )
    {
	if (alt_inlu == stdin)
	{
	    if (infile == NULLADDR) (void) fprintf(stdout,"input> ");
	    if (char_gets(input, 100, inlu) == NULL)
	    {
		if (infile != NULLADDR)
		{
		    fprintf(stderr,
"EDCHAR: EOF or error reading line from %s.\n",infile);
		    exit (4);
		}
	    }
	}
	else
	{
	    if (char_gets(input, 100, alt_inlu) == NULL)
	    {
		fprintf(stderr,
"EDCHAR: EOF or error reading line from alternate input.\n");
		if (infile != NULLADDR)
		{
		    exit (4);
		}
		(void) fclose(alt_inlu);
		alt_inlu = stdin;
		*input = '\0';
	    }
	}
	cptr = input;
  	if (echo_flag != ECHO_NO) fprintf(stderr,"%s\n",input);
	switch ((key = get_comm(comlev1, 4L, &cptr)))
	{
	case L1_SET:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = set(&cptr);
	    break;
	case L1_GET:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = get(&cptr);
	    break;
	case L1_PUT:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = put(&cptr);
	    break;
	case L1_LINE:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = line(&cptr);
	    break;
	case L1_WINDOW:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = window(&cptr);
	    break;
	case L1_ERASE:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = erase(&cptr);
	    break;
	case L1_DISPLAY:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = display(&cptr, outlu, scale_flag);
	    break;
	case L1_FILL:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = fill(&cptr);
	    break;
	case L1_QUIT:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    if (alt_inlu == stdin)
	    {
		fprintf(stdout,"Good Bye !!!\n");
		flag = 0;
		status = 1;
	    }
	    else
	    {
		fprintf(stdout,"Closing alternate input.\n");
		(void) fclose(alt_inlu);
		alt_inlu = stdin;
	    }
	    break;
	case L1_HELP:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = help(&cptr);
	    break;
	case L1_MOVE:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = move(&cptr);
	    break;
	case L1_FILE:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = file(&cptr, &outlu, scale_flag);
	    break;
	case L1_EXECUTE:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = execute(&cptr, &alt_inlu);
	    break;
	case L1_SHELL:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = shell(&cptr);
	    break;
	case L1_COMMENT:
	    if (echo_flag == ECHO_WAIT) (void) sleep(1);
	    status = 1;
	    break;
	case L1_ECHO:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = echo(&cptr, &echo_flag);
	    break;
	case L1_SCALE:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = scale(&cptr, &scale_flag);
	    break;
	case L1_ARC:
#ifdef DEBUG
	    fprintf(stdout,"Command is %s.\n", comlev1[key].com);
#endif
	    status = arc(&cptr);
	    break;
	case TOOMANY:
	    if (alt_inlu != stdin)
	    {
		fprintf(stderr,
"EDCHAR: error in command from alternate input: %s\n",input);
		fprintf(stderr,"Too many matches, try again.\n");
		if (infile != NULLADDR) exit (4);
		fprintf(stderr,"Alternate file closed..\n");
		(void) fclose(alt_inlu);
		alt_inlu = stdin;
	    }
	    else if (infile != NULLADDR)
	    {
		fprintf(stderr,"EDCHAR: error in command: %s\n",input);
		fprintf(stderr,"Too many matches, try again.\n");
		exit (4);
	    }
	    fprintf(stderr,"Too many matches, try again.\n");
	    status = 1;
	    break;
	case BLANKLINE:
	    status = 1;
	    break;
	case NOMATCH:
	default:
	    if (alt_inlu != stdin)
	    {
		fprintf(stderr,
"EDCHAR: error in command from alternate input: %s\n",input);
		fprintf(stderr,"No match, try again.\n");
		if (infile != NULLADDR) exit (4);
		fprintf(stderr,"Alternate file closed..\n");
		(void) fclose(alt_inlu);
		alt_inlu = stdin;
	    }
	    else if (infile != NULLADDR)
	    {
		fprintf(stderr,"EDCHAR: error in command: %s\n",input);
		fprintf(stderr,"No match, try again.\n");
		exit (4);
	    }
	    fprintf(stderr,"No match, try again.\n");
	    status = 1;
	    break;
	}
	if (! status)
	{
	    fprintf(stderr,"EDCHAR: error in command: %s\n",input);
	    fprintf(stderr,"Try again.\n");
	    if (infile != NULLADDR) exit (4);
	    if (alt_inlu != stdin)
	    {
		(void) fclose(alt_inlu);
	 	alt_inlu = stdin;
	    }
	}
    }
    if (infile != NULLADDR)
    {
	fclose(inlu);
    }
    if (outfile != NULLADDR)
    {
	(void) char_writescr(outlu, scale_flag);
	fclose(outlu);
    }
    else if (infile != NULLADDR)
    {
	(void) char_writescr(outlu, scale_flag);
    }
    exit (0);
}
