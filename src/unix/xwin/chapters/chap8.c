// demo for chapter 7

// headers
#include "myxlib.h"

// font names
#define FONT_NAME "variable"
#define FALLBACK_FONT_NAME "fixed"

// global colors
unsigned long black, white;

// cursor names 
char *cursor_names[] = {
	"XC_X_cursor",
	"XC_arrow",
	"XC_based_arrow_down",
	"XC_based_arrow_up",
	"XC_boat",
	"XC_bogosity",
	"XC_bottom_left_corner",
	"XC_bottom_right_corner",
	"XC_bottom_side",
	"XC_bottom_tee",
	"XC_box_spiral",
	"XC_center_ptr",
	"XC_circle",
	"XC_clock",
	"XC_coffee_mug",
	"XC_cross",
	"XC_cross_reverse",
	"XC_crosshair",
	"XC_diamond_cross",
	"XC_dot",
	"XC_dotbox",
	"XC_double_arrow",
	"XC_draft_large",
	"XC_draft_small",
	"XC_draped_box",
	"XC_exchange",
	"XC_fleur",
	"XC_gobbler",
	"XC_gumby",
	"XC_hand1",
	"XC_hand2",
	"XC_heart",
	"XC_icon",
	"XC_iron_cross",
	"XC_left_ptr",
	"XC_left_side",
	"XC_left_tee",
	"XC_leftbutton",
	"XC_ll_angle",
	"XC_lr_angle",
	"XC_man",
	"XC_middlebutton",
	"XC_mouse",
	"XC_pencil",
	"XC_pirate",
	"XC_plus",
	"XC_question_arrow",
	"XC_right_ptr",
	"XC_right_side",
	"XC_right_tee",
	"XC_rightbutton",
	"XC_rtl_logo",
	"XC_sailboat",
	"XC_sb_down_arrow",
	"XC_sb_h_double_arrow",
	"XC_sb_left_arrow",
	"XC_sb_right_arrow",
	"XC_sb_up_arrow",
	"XC_sb_v_double_arrow",
	"XC_shuttle",
	"XC_sizing",
	"XC_spider",
	"XC_spraycan",
	"XC_star",
	"XC_target",
	"XC_tcross",
	"XC_top_left_arrow",
	"XC_top_left_corner",
	"XC_top_right_corner",
	"XC_top_side",
	"XC_top_tee",
	"XC_trek",
	"XC_ul_angle",
	"XC_umbrella",
	"XC_ur_angle",
	"XC_watch",
	"XC_xterm",
};

// events to receive
#define EVENT_MASK (ButtonPressMask|KeyPressMask|ExposureMask)

// get cursor name 
void
GetCursorName(int cursor_number, char *name)
{
	int cursor_index;
	cursor_index = cursor_number/2;

	if (cursor_index < 0)
		cursor_index = 0;

	if (cursor_index > (XC_num_glyphs-2))
		cursor_index = XC_num_glyphs - 2;

	strcpy(name, cursor_names[cursor_index]);

	return;
}

// draw the shit
void
Redraw(Display *display, Window window, GC gc, 
	XFontStruct *font_struct, int cursor_number)
{
	int x, y, height;
	char string[100], name[100];

	height = FontHeight(font_struct) + 1;
	x = 30;
	y = 30;

	DrawString(display, window, gc, x, y, 
		"click mouse button to change cursor.");
	y += height;

	DrawString(display, window, gc, x, y, "press qlt-q to quit.");
	y += height;

	GetCursorName(cursor_number, name);

	sprintf(string, "%s, number %d/%d",
		name, cursor_number, (XC_num_glyphs - 2));
	DrawString(display, window, gc, x, y, string);

	XFlush(display);
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

	// create initial cursor
	int cursor_number = XC_X_cursor;
	Cursor cursor = XCreateFontCursor(display, cursor_number);
	if (cursor != (Cursor)None)
	{
		XDefineCursor(display, window, cursor);
	}

	// map window
	XMapRaised(display, window);
	XFlush(display);

	// event loop
	KeySym keysym;
	XEvent event;
	for (int done = False; done != True; )
	{
		XNextEvent(display, &event);
		switch (event.type)
		{
		case Expose:
			if (event.xexpose.count == 0)
			{
				Redraw(display, window, gc, 
					font_struct, cursor_number);
			}
			break;
		case ButtonPress:
			cursor_number += 2;
			if (cursor_number > XC_xterm)
				cursor_number = 0;
			if (cursor != (Cursor)None)
			{
				XFreeCursor(display, cursor);
			}
			cursor = XCreateFontCursor(display, cursor_number);
			if (cursor != (Cursor)None)
			{
				XDefineCursor(display, window, cursor);
			}
			XClearWindow(display, window);
			Redraw(display, window, gc, font_struct, cursor_number);
			break;
		case KeyPress:
			done = DecodeKeyPress(&event.xkey, &keysym, string);
			break;
		case MappingNotify:
			XRefreshKeyboardMapping(&event.xmapping);
			break;
		}
	}

	// close connection
	XFreeFont(display, font_struct);
	XCloseDisplay(display);

	// all done
	return(0);
}
