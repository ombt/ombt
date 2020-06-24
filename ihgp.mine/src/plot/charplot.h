#ifndef __CHARPLOT_H
#define __CHARPLOT_H
/*
 * plot functions header file.
 */
extern long char_setscr();
extern long char_setback();
extern long char_setfore();
extern long char_getback();
extern long char_getfore();
extern long char_erase();
extern long char_paintw();
extern long char_setcursor();
extern long char_getcursor();
extern long char_putsym();
extern long char_putpoint();
extern long char_putsymp();
extern long char_putstring();
extern long char_putstringp();
extern long char_putline();
extern long char_putlinep();
extern long char_getmaxscr();
extern long char_getscr();
extern long char_pointcolor();
extern long char_fill();
extern long char_arc();

#define MAXX 201L
#define MAXY 201L

extern long MAXPX;
extern long MAXPY;

extern char screen[MAXY][MAXX];

#define MAXLEVEL 11L
#define MAXCOLOR MAXLEVEL - 1L

extern char palette[MAXLEVEL];

extern long cursorx, cursory;

#define VERTICAL 1L
#define HORIZONTAL 2L
#define UP 4L
#define DOWN 8L
#define LEFT 16L
#define RIGHT 32L
#define LINE 64L
#define POINT 128L
#define OUTSIDE 256L
#define ASCII 512L
#define GRAY 1024L
#define OWASCII 2048L
#define ASCIIBND 4096L

#endif
