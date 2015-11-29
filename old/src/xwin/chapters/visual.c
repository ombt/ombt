// headers
#include "myxlib.h"

// set up visual info
int
SetUpVisual(Display *display, int screen, Visual *&visual, int &depth)
{
	int number_visuals;

	XVisualInfo *visual_array;
	XVisualInfo visual_info_template;
	int status = False;

	// look for a pseudo-color visual
	if (DefaultVisual(display, screen)->c_class == PseudoColor)
	{
		// found a pseudo-color visual
		visual = DefaultVisual(display, screen);
		depth = DefaultDepth(display, screen);
		status = True;
	}
	else
	{
		// looking for a pseudo-color visual
		visual_info_template.c_class = PseudoColor;
		visual_info_template.screen = screen;
		visual_array = XGetVisualInfo(display, 
			(VisualClassMask | VisualScreenMask),
			&visual_info_template, &number_visuals);
		if ((number_visuals > 0) && (visual_array != NULL))
		{
			visual = visual_array[0].visual;
			depth = visual_array[0].depth;
			XFree(visual_array);
			status = True;
		}
		else
		{
			visual = CopyFromParent;
			status = False;
		}
	}

	// all done
	return(status);
}
