// demo for chapter 7

// headers
#include "myxlib.h"

// font names
#define FONT_NAME "variable"
#define FALLBACK_FONT_NAME "fixed"

// global colors
unsigned long black, white;

// button ids
char *ButtonIds[] = {
	"no button",
	"button 1",
	"button 2",
	"button 3",
	"button 4",
	"button 5"
};

// events to receive
#define EVENT_MASK \
	(ButtonPressMask | \
	ButtonReleaseMask | \
	EnterWindowMask | \
	LeaveWindowMask | \
	KeyPressMask | \
	ExposureMask | \
	StructureNotifyMask)

// draw the shit
void
redraw(Display *display, Window window, GC gc)
{
	DrawString(display, window, gc, 30, 30, 
		"chapter 7: enter alt-q to quit.");
	XFlush(display);

	return;
}

// handle events
void
HandleKeyPress(XKeyEvent *event, KeySym keysym, char *keybuffer)
{
	if ((event->state & Mod1Mask) ||
	    (event->state & Mod2Mask))
	{
		cout << "META-" << keybuffer << " was hit." << endl;
	}
	else
	{
		// if (isascii(keysym) && (! iscntrl(keysym)))
		// {
			// cout << "KeyPress <" << keybuffer << "> key" << endl;
		// }
		// else
		// {
			cout << "KeyPress <" << XKeysymToString(keysym) << "> key" << endl;
		// }
	}
	AppendKeyStateMessage(event->state);
	return;
}

// main routine
main(int argc, char **argv)
{
	char string[BUFSIZ];

	// connect to x-server
	int screen;
	Window rootwindow;
	Display *display = ConnectToServer(NULL, screen, rootwindow);
	if (display == NULL)
	{
		ERROR("connect to server failed.", EINVAL);
		return(2);
	}

	// get basic colors
	black = BlackPixel(display, screen);
	white = WhitePixel(display, screen);

	// create a window
	Visual *visual = CopyFromParent;
	int x = 10;
	int y = 10;
	int width = 300;
	int height = 300;
	Window window = OpenWindow(display, 
			rootwindow,
			x, y, width, height,
			black, white,
			EVENT_MASK, visual);

	// set hints for window
	SetStandardHints(display, window, argv[0], argv[0],
			x, y, width, height);

	// create GC for drawing
	GC gc = CreateGC(display, window, black, white);

	// load a font
	XFontStruct *font_struct = 
		LoadFont(display, FONT_NAME, FALLBACK_FONT_NAME);

	// set up gc with font structure
	XSetFont(display, gc, font_struct->fid);

	// map window
	XMapRaised(display, window);
	XFlush(display);

	// event loop: max of 20 events, then exit.
	KeySym keysym;
	XEvent event;
	for (int done = False; done != True; )
	{
		XNextEvent(display, &event);
		switch (event.type)
		{
		case Expose:
			cout << "Expose: <x, y, w, h> = <";
			cout << event.xexpose.x << ",";
			cout << event.xexpose.y << ",";
			cout << event.xexpose.width << ",";
			cout << event.xexpose.height;
			cout << ">" << endl;
			if (event.xexpose.count == 0)
			{
				cout << "last expose event." << endl;
			}
			break;
		case MapNotify:
			cout << "MapNotify: window was mapped." << endl;
			break;
		case ButtonPress:
			cout << "ButtonPress: ";
			cout << ButtonIds[event.xbutton.button];
			cout << " at <";
			cout << event.xbutton.x << ",";
			cout << event.xbutton.y << ">" << endl;
			AppendKeyStateMessage(event.xbutton.state);
			break;
		case EnterNotify:
			cout << "EnterNotify: pointer enters window" << endl;
			break;
		case LeaveNotify:
			cout << "LeaveNotify: pointer enters window" << endl;
			break;
		case ButtonRelease:
			cout << "ButtonRelease: ";
			cout << ButtonIds[event.xbutton.button];
			cout << " at <";
			cout << event.xbutton.x << ",";
			cout << event.xbutton.y << ">" << endl;
			AppendKeyStateMessage(event.xbutton.state);
			break;
		case KeyPress:
			done = DecodeKeyPress(&event.xkey, &keysym, string);
			HandleKeyPress(&event.xkey, keysym, string);
			break;
		case ConfigureNotify:
			cout << "ConfigureNotify: <x, y, w, h> = <";
			cout << event.xconfigure.x << ",";
			cout << event.xconfigure.y << ",";
			cout << event.xconfigure.width << ",";
			cout << event.xconfigure.height;
			cout << ">" << endl;
			break;
		}

		// redraw for all events.
		if ( ! done) redraw(display, window, gc);
	}

	// close connection
	XFreeFont(display, font_struct);
	XCloseDisplay(display);

	// all done
	return(0);
}
