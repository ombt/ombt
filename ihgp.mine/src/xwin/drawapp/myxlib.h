#ifndef __MYXLIB_H
#define __MYXLIB_H
/* headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* function externs */
extern void PrintXInfo();
extern void SetClassHints();
extern Display *ConnectToServer();
extern void FillSizeHints();
extern void SetSizeHints();
extern Window OpenWindow();
extern void SetStandardHints();
extern void SetWMHints();
extern void SetWindowName();
extern GC CreateGC();
extern void DrawOval();
extern void FillOval();
extern unsigned long AllocNamedColor();
extern int SetUpColormap();
extern int SetUpVisual();
extern int FontHeight();
extern XFontStruct *LoadFont();
extern void DrawString();
extern void AppendKeyStateMessage();
extern int DecodeKeyPresS();
extern int IsMetaKey();
extern int IsMetaQ();
extern void UpdateWindowFromBitmap();
extern void CopyPlane();
extern Pixmap LoadBitmap();
extern int SaveBitmap();
extern void LoadIcon();
extern int SetWindowIcon();
extern GC CreateXorGC();
extern int FindDisplayName();
extern Display *OpenDisplay();
extern int FindGeometry();
extern void ParseGeometry();
extern void CheckGeometry();
extern int CheckForHelp();
extern void PrintUsageMessage();
extern char *FindParameter();
extern XFontStruct *LoadFontName();
extern Window TopWindow();
extern int FindInitialState();
extern void FindWindowName();
extern Display *StartupX();
extern void SetErrorHandlers();
extern int ErrorHandler();
extern int FatalErrorHandler();

#endif
