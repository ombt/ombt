#ifndef __SPACES_H
#define __SPACES_H

#include <stdio.h>
#include <string.h>

class Spaces {
public:
	Spaces() {
		// do nothing
	}
	~Spaces() {
		// do nothing
	}

	// increment/decrement operators
	Spaces &operator+=(int incr) {
		numberofspaces += incr;
		return(*this);
	}
	Spaces &operator-=(int decr) {
		numberofspaces -= decr;
		return(*this);
	}

	// access operator
	const char *operator()() const {
		int i=0;
		for ( ; i<BUFSIZ && i<numberofspaces; i++)
		{
			spaces[i] = ' ';
		}
		spaces[i] = '\0';
		return(spaces);
	}

private:
	static int numberofspaces;
	static char spaces[BUFSIZ+1];
};

#endif
