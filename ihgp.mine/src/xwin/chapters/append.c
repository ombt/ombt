// headers
#include "myxlib.h"

// convert event to string
void
AppendKeyStateMessage(int state)
{
	char string[BUFSIZ];

	// convert state to a string 
	strcpy(string, "    ");
	if (state & Button1Mask)
	{
		strcat(string, "Button1 ");
	}
	if (state & Button2Mask)
	{
		strcat(string, "Button2 ");
	}
	if (state & Button3Mask)
	{
		strcat(string, "Button3 ");
	}
	if (state & Button4Mask)
	{
		strcat(string, "Button4 ");
	}
	if (state & Button5Mask)
	{
		strcat(string, "Button5 ");
	}
	if (state & ShiftMask)
	{
		strcat(string, "Shift ");
	}
	if (state & LockMask)
	{
		strcat(string, "Lock ");
	}
	if (state & ControlMask)
	{
		strcat(string, "Control ");
	}
	if (state & Mod1Mask)
	{
		strcat(string, "Mod1 ");
	}
	if (state & Mod2Mask)
	{
		strcat(string, "Mod2 ");
	}
	if (state & Mod3Mask)
	{
		strcat(string, "Mod3 ");
	}
	if (state & Mod4Mask)
	{
		strcat(string, "Mod4 ");
	}
	if (state & Mod5Mask)
	{
		strcat(string, "Mod5 ");
	}

	/* print message */
	if (strlen(string) > 4) cout << string << endl;

	/* all done */
	return;
}
