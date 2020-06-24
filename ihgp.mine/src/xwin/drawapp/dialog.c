/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"

/* definitions */
#define BUTTON_WIDTH 60

/* globals */
Window dialog_window = (Window)None;
int dial1_button = -1;
int dial2_button = -1;
int dial_label = -1;
int dial_edit = -1;
int dialog_up = False;

/* colors */
unsigned long black, white, lightgrey;

/* get mouse location */
Window
CreateDialog(display, parent, owner, visual, colormap, font_struct,
		app_name, prompt, callback)
Display *display;
Window parent, owner;
Visual *visual;
Colormap colormap;
XFontStruct *font_struct;
char *app_name;
char *prompt;
void (*callback)();
{
	int x, y, width, height;
	Window window;
	GC gc;
	int button_height;

	/* get location of mouse */
	QueryPointer(display, &x, &y);
	width = BUTTON_WIDTH*3;
	height = FontHeight(font_struct)*7;

	/* create window for dialog */
	window = CreateTransientWindow(display, parent, owner,
			x, y, width, height, black, lightgrey,
			KeyPressMask, visual, colormap, 
			app_name, "Pop-Up Dialog");

	/* create graphics */
	gc = CreateGC(display, window, black, white);
	XSetFont(display, gc, font_struct->fid);

	/* create label for prompt */
	button_height = FontHeight(font_struct)+10;
	dial_label = CreateTextLabel(display, window, 6, 5, 
			(width-12), button_height, gc, font_struct, prompt);

	/* create edit area */
	dial_edit = CreateTextEntry(display, window, 6, (button_height+10),
			(width-12), button_height, gc, font_struct, 
			"", callback);

	/* create push buttons */
	dial1_button = CreatePushButton(display, window, 6, 
				(height - button_height - 10),
				BUTTON_WIDTH, button_height, gc,
				font_struct, "OK", callback);
	dial2_button = CreatePushButton(display, window,
				(width - BUTTON_WIDTH - 10),
				(height - button_height - 10),
				BUTTON_WIDTH, button_height, gc,
				font_struct, "Cancel", PopDownDialog);
	dialog_window = window;

	/* all done */
	return(window);
}

/* remove dialog window */
void
PopDownDialog(id)
int id;
{
	Display *display;
	if (id >= 0) UnhighlightPushButton(id);
	if ((dialog_window != (Window)None) && (dialog_up == True))
	{
		display = ToolGetDisplay(id);
		XUnmapWindow(display, dialog_window);
		XFlush(display);
		dialog_up = False;
	}
	return;
}

/* make dialog window visible */
void
PopUpDialog()
{
	Display *display;
	int x, y;
	if ((dialog_window != (Window)None) && (dialog_up == False))
	{
		display = ToolGetDisplay(0);
		QueryPointer(display, &x, &y);
		XMoveWindow(display, dialog_window, x, y);
		XMapRaised(display, dialog_window);
		XMapSubwindows(display, dialog_window);
		XFlush(display);
		dialog_up = True;
	}
	return;
}

/* get data */
void
GetDialogData(data)
char *data;
{
	if (dial_edit > 0) 
		ToolGetName(dial_edit, data);
	return;
}

/* set call back function */
void
SetDialogCallback(callback)
void (*callback)();
{
	if (dial1_button >= 0)
		ToolSetUserFunc(dial1_button, callback);
	if (dial_edit >= 0)
		ToolSetUserFunc(dial_edit, callback);
	return;
}

/* cancel call back */
void
SetDialogCancelCallback(callback)
void (*callback)();
{
	if (dial2_button >= 0)
		ToolSetUserFunc(dial2_button, callback);
	return;
}

/* set data */
void
SetDialogData(new_data)
char *new_data;
{
	if (dial_edit >= 0)
	{
		ToolSetName(dial_edit, new_data);
		ToolClear(dial_edit);
		ToolRedraw(dial_edit);
	}
	return;
}

void
SetDialogPrompt(prompt)
char *prompt;
{
	if (dial_label >= 0)
	{
		ToolSetName(dial_label, prompt);
		ToolClear(dial_label);
		ToolRedraw(dial_label);
	}
	return;
}
