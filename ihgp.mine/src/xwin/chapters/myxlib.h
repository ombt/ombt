#ifndef __MYXLIB_H
#define __MYXLIB_H

// operating system headers
#include <stdio.h>
#include <stdlib.h>
#include <sysent.h>
#include <iostream.h>
#include <ctype.h>

// X window headers
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

// local headers
#include "returns.h"
#include "debug.h"

// externs for library functions
extern void SetClassHints(Display *, Window , char *, char *);
extern Display *ConnectToServer(char *, int &, Window &);
extern void FillSizeHints(int, int, int, int, XSizeHints &);
extern void SetSizeHints(Display *, Window, int, int, int, int );
extern Window OpenWindow(Display *, Window, int, int, int, int, unsigned long, unsigned long, unsigned long, Visual *);
extern void SetStandardHints(Display *, Window, char *, char *, int, int, int, int);
extern void SetWMHints(Display *, Window, int);
extern void SetWindowName(Display *, Window, char *);
extern void SetWindowNameWithTextProps(Display *, Window, char *);
extern void DrawOval(Display *, Drawable, GC, int, int, int, int);
extern void FillOval(Display *, Drawable, GC, int, int, int, int);
extern GC CreateGC(Display *, Drawable, unsigned long, unsigned long);
extern int SetUpColormap(Display *, int, Window, Visual *, Colormap &);
extern unsigned long AllocNamedColor(Display *, Colormap, char *, unsigned long);
extern int SetUpVisual(Display *, int, Visual *&, int &);
extern int FontHeight(XFontStruct *);
extern XFontStruct *LoadFont(Display *, char *, char *);
extern void DrawString(Display *, Window, GC, int, int, char *);
extern void AppendKeyStateMessage(int);
extern int DecodeKeyPress(XKeyEvent *, KeySym *, char *);
extern int IsMetaKey(XKeyEvent *);
extern int IsMetaQ(XKeyEvent *, char *);
extern void UpdateWindowFromBitmap(Display *, Window, GC, Pixmap, XExposeEvent *, int, int );
extern void CopyPlane(Display *, Drawable, Drawable, GC, int, int, int, int, int, int );
extern Pixmap LoadBitmap(Display *, Window, char *, int *, int *);
extern int SaveBitmap(Display *, char *, Pixmap, int, int );
extern void LoadIcon(Display *, Window, char *, char *);
extern int SetWindowIcon(Display *, Window, Pixmap, char *);
extern GC CreateXorGC(Display *, Drawable , unsigned long , unsigned long );
#endif

