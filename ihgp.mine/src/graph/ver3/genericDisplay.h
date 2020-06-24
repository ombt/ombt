#ifndef __GENERIC_DISPLAY_H
#define __GENERIC_DISPLAY_H
/* abstract class for a generic window display */

/* required headers */
#include <sysent.h>
#include <stdlib.h>
#include <string.h>

/* define class */
class GenericDisplay;

/* generic display class definition */
class GenericDisplay {
public:
	/* constructor and destructor */
	GenericDisplay(char * = (char *)0);
	GenericDisplay(const GenericDisplay &);
	virtual ~GenericDisplay();

	/* initialization functions */
	virtual int init(char * = (char *)0);
	virtual int init(const GenericDisplay &);
	virtual int deleteAll();

	/* operators */
	GenericDisplay &operator=(const GenericDisplay &);

	/* miscellaneous */
	inline int getStatus() { return(status); }

protected:
	/* internal data */
	int status;
	char *displayName;
};

#endif
