// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *			Service C++ functions 
 *	     that support the standard environment for me
 *
 * $Id: myenv.cc,v 1.4 1995/06/05 16:17:52 oleg Exp oleg $
 */

#ifdef __GNUC__
#pragma implementation
#endif

#include "myenv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
 *-----------------------------------------------------------------------
 *		Some global constant pertaining to input/output
 */

const char _Minuses [] = "\
-------------------------------------------------------------------------------";

const char _Asteriscs [] = "\
*******************************************************************************";

const char _Equals [] = "\
===============================================================================";


/*
 *------------------------------------------------------------------------
 *	        Print an error message at stderr and abort
 * Synopsis
 *	volatile void _error(const char * message,... );
 *	Message may contain format control sequences %x. Items to print 
 *	with the control sequences are to be passed as additional arguments to
 *	the function call.
 */

volatile void _error(const char * message,...)
{
  va_list args;
  va_start(args,message);		/* Init 'args' to the beginning of */
					/* the variable length list of args*/
  fprintf(stderr,"\n_error:\n"); 	
  vfprintf(stderr,message,args);
  fputs("\n",stderr);
#ifdef __MWERKS__
  exit(4);
#else  
  abort();
#endif
}


/*
 *------------------------------------------------------------------------
 *	       		 Print a message at stderr
 * Synopsis
 *	void message(const char * text,... );
 *	Message may contain format control sequences %x. Items to print 
 *	with the control sequences are to be passed as additional arguments to
 *	the function call.
 */

void message(const char * text,...)
{
  va_list args;
  va_start(args,text);		/* Init 'args' to the beginning of */
					/* the variable length list of args*/
  vfprintf(stderr,text,args);
}

//------------------------------------------------------------------------
//		    Patches to the standard environment

                                // Like strncpy(), but ALWAYS terminates
                                // the destination string
char * xstrncpy(char * dest, const char * src, const int len)
{
  strncpy(dest,src,len);
  dest[len] = '\0';
  return dest;
}

#ifndef __GNUC__
#include <time.h>

				// libg++ nifty timing functions
				// Very rough and dirty implementation for
				// platforms w/o libg++
static time_t time_set;

double start_timer(void)
{
  return time_set = time(0);
}
				// return_elapsed_time(last_time) returns
				// process time (in secs) since Last_Time
				// If Last_time == 0.0, return time since
				// the last call to start_timer()
double return_elapsed_time(const double last_time)
{
  time_t new_time = time(0);
  if( time_set == 0 )
    return -1;				// timer wasn't started
  return new_time - (last_time == 0.0 ? time_set : last_time);
}

#endif
