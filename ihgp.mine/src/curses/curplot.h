#ifndef __CURPLOT_H
#define __CURPLOT_H

#define CHECKBITS(flag, bitpat) (((flag) & (bitpat)) == (bitpat))
#define SETBITS(flag, bitpat) (flag) = ((flag) | (bitpat))
#define CLRBITS(flag, bitpat) (flag) = ((flag) & ~(bitpat))

#define MAXLEVEL 11L
#define MAXCOLOR MAXLEVEL - 1L
#define NEWLINE ((unsigned char) '\n')

typedef unsigned char UCHAR;
typedef unsigned short USHORT;

struct cur_window
{
    WINDOW *cwin;
    UCHAR ascii2gray[128];
    UCHAR palette[MAXLEVEL];
    UCHAR **gray;
    UCHAR *spare;
    long px;
    long py;
    long maxpx;
    long maxpy;
    long flags;
};
typedef struct cur_window CUR_WINDOW;

#define CUR_NONE 	0L
#define CUR_NL		1L			/* screen */
#define CUR_CBREAK	2L*CUR_NL
#define CUR_ECHO	2L*CUR_CBREAK
#define CUR_RAW		2L*CUR_ECHO
#define CUR_STANDOUT	2L*CUR_RAW
#define CUR_REVERSE	2L*CUR_STANDOUT		/* video */
#define CUR_BOLD	2L*CUR_REVERSE
#define CUR_DIM		2L*CUR_BOLD
#define CUR_BLINK	2L*CUR_DIM
#define CUR_UNDERLINE	2L*CUR_BLINK
#define CUR_CLEAROK	2L*CUR_UNDERLINE
#define CUR_IDLOK	2L*CUR_CLEAROK		/* CURSES options */
#define CUR_KEYPAD	2L*CUR_IDLOK
#define CUR_LEAVEOK	2L*CUR_KEYPAD
#define CUR_META	2L*CUR_LEAVEOK
#define CUR_DELAY	2L*CUR_META
#define CUR_INTRFLUSH	2L*CUR_DELAY
#define CUR_SCROLLOK	2L*CUR_INTRFLUSH
#define CUR_REFRESH	2L*CUR_SCROLLOK		/* curplot options */
#define CUR_TOUCHW	2L*CUR_REFRESH

#define LINE 		1L
#define POINT 		2L*LINE
#define OUTSIDE 	2L*POINT
#define ASCII		2L*OUTSIDE
#define GRAY		2L*ASCII
#define OWASCII		2L*GRAY
#define ASCIIBND	2L*OWASCII

extern void cur_reset();
extern void cur_initscr();
extern void cur_exitscr();
extern long cur_getwindow();
extern long cur_delwindow();
extern long cur_setcursor();
extern long cur_getcursor();
extern long cur_putpoint();
extern long cur_putpointp();
extern long cur_putsym();
extern long cur_putsymp();
extern long cur_putstring();
extern long cur_putstringp();
extern long cur_setback();
extern long cur_getback();
extern long cur_setfore();
extern long cur_getfore();
extern long cur_erase();
extern long cur_harderase();
extern long cur_paintw();
extern long cur_putarc();
extern long cur_putline();
extern long cur_putlinep();
extern void cur_fill();
extern long cur_arc();
extern long cur_setterm();
extern long cur_setwoption();
extern long cur_options();
extern long cur_setwvideo();
extern long cur_write();
extern long cur_scroll();
extern long cur_setscrollreg();
extern long cur_box();
extern long cur_printw();
extern long cur_filein();
extern long cur_fileout();
extern long cur_chglvl();
extern long cur_encode();

#endif
