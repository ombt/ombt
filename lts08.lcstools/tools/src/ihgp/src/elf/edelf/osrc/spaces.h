#ifndef __SPACES_H
#define __SPACES_H

#include <stdio.h>
#include <string.h>

class Spaces {
public:
	Spaces(int us = 1) {
		usestatic = us;
	}
	~Spaces() {
		// do nothing
	}

	// increment/decrement operators
	Spaces &operator+=(int incr) {
		if (usestatic)
			s_numberofspaces += incr;
		else
			numberofspaces += incr;
		return(*this);
	}
	Spaces &operator-=(int decr) {
		if (usestatic)
			s_numberofspaces -= decr;
		else
			numberofspaces -= decr;
		return(*this);
	}

	// access operator
	const char *operator()() {
		int i=0;
		if (usestatic)
		{
			for ( ; i<BUFSIZ && i<s_numberofspaces; i++)
			{
				s_spaces[i] = ' ';
			}
			s_spaces[i] = '\0';
			return(s_spaces);
		}
		else
		{
			for ( ; i<BUFSIZ && i<numberofspaces; i++)
			{
				spaces[i] = ' ';
			}
			spaces[i] = '\0';
			return(spaces);
		}
	}

private:
	// static case
	static int s_numberofspaces;
	static char s_spaces[BUFSIZ+1];

	// private case
	int usestatic;
	int numberofspaces;
	char spaces[BUFSIZ+1];
};

#endif
