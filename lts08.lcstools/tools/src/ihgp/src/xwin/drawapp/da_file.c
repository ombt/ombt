/* headers */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"
#include "drawapp.h"

/* definitions */
#define MAX_CHARS 400

/* globals */
char da_filename[MAX_CHARS+1];

void
PopUpLoadFile(id)
int id;
{
	/* set up dialog box for user */
	SetDialogData("");
	SetDialogPrompt("enter name of file to load");
	SetDialogCallback(LoadFile);

	/* display box */
	PopUpDialog();
	UnhighlightPushButton(id);
	return;
}

void
LoadFile(id)
int id;
{
	char filename[200];
	extern Window main_window;

	UnhighlightPushButton(id);
	PopDownDialog(-1);
	GetDialogData(filename);
	LoadDrawappFile(ToolGetDisplay(id), main_window, filename);
	return;
}

void
LoadDrawappFile(display, window, filename)
Display *display;
Window window;
char *filename;
{
	int width, height;
	Pixmap new_bitmap;
	extern Pixmap drawing_pixmap;
	extern GC pixmap_gc;
	extern int editing_width, editing_height;

	new_bitmap = LoadBitmap(display, window, filename, &width, &height);
	if (new_bitmap == (Pixmap) None)
	{
		ERRORS("error loading bitmap file.", filename, EINVAL);
		return;
	}
	if ((editing_width != width) || (editing_height != height))
	{
		DestroyUndoPixmap(display);
		DestroyDrawPixmap(display);
		CreateUndoPixmap(display, window, width, height);
		CreateDrawingPixmap(display, window, width, height);
		ResizeEditingWindow(width, height);
	}
	CopyPlane(display, new_bitmap, drawing_pixmap, pixmap_gc,
		0, 0, width, height, width, height);
	XFlush(display);
	XFreePixmap(display, new_bitmap);
	ToolRedraw(EDITING_WINDOW);
	SetFileName(display, window, filename);
	XFlush(display);
	return;
}

void
CheckForFileLoad(display, window, argc, argv)
Display *display;
Window window;
int argc;
char **argv;
{
	char *filename;
	filename = FindParameter(argc, argv, "-file");
	if (filename == (char *)NULL)
	{
		filename = FindParameter(argc, argv, "-load");
	}
	if (filename != (char *)NULL)
	{
		LoadDrawappFile(display, window, filename);
	}
	return;
}

void
SaveFile(id)
int id;
{
	extern int editing_width, editing_height;
	extern Pixmap drawing_pixmap;
	extern int changed_flag;
	int status;
	Display *display;

	display = ToolGetDisplay(id);
	GetDialogData(da_filename);
	status = SaveBitmap(display, da_filename, drawing_pixmap,
			editing_width, editing_height);
	if (status != BitmapSuccess)
	{
		ERRORS("error in saving bitmap to file.", da_filename, errno);
	}
	else
	{
		changed_flag = BITMAP_SAVED;
	}
	UnhighlightPushButton(id);
	return;
}

void
SetFileName(display, window, filename)
Display *display;
Window window;
char *filename;
{
	char window_name[400];
	extern int changed_flag;

	strncpy(da_filename, filename, MAX_CHARS);
	da_filename[MAX_CHARS] = 0;
	SetDialogData(da_filename);
	sprintf(window_name, "DrawApp: %s", da_filename);
	SetWindowName(display, window, window_name);
	changed_flag = BITMAP_CHANGED;
	return;
}

void
Quit(id)
int id;
{
	extern int changed_flag;
	static int quit_flag = 0;

	if (quit_flag > 0) SaveFile(id);
	if (changed_flag == BITMAP_CHANGED)
	{
		SetDialogData(da_filename);
		SetDialogPrompt("File not saved. Save it?");
		SetDialogCallback(Quit);
		SetDialogCancelCallback(ReallyQuit);
		quit_flag = 1;
		PopUpDialog();
	}
	else
	{
		exit(0);
	}
}

void
ReallyQuit(id)
int id;
{
	exit(0);
}
