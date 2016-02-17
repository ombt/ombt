/* headers */
#include <stdio.h>
#include <X11/Xlib.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"
#include "toolkit.h"
#include "drawapp.h"

/* handle meta keys */
void
HandleMetaKey(string)
char *string;
{
	XEvent event;
	KeySym keysym;
	int id = -1;
	switch (string[0])
	{
	case 'F':
	case 'f':
		break;
	case 'L':
	case 'l':
		break;
	case 'O':
	case 'o':
		break;
	case 'P':
	case 'p':
		break;
	case 'Q':
	case 'q':
		break;
	case 'R':
	case 'r':
		break;
	case 'S':
	case 's':
		break;
	case 'T':
	case 't':
		break;
	case 'U':
	case 'u':
		break;
	}
	if (id >= 0)
	{
		keysym = 0;
		event.type = ButtonPress;
		PushButtonEvent(ToolGetDisplay(id), id, &event,
				keysym, string);
	}
	return;
}

