// This may look like C code, but it is really -*- C++ -*-
//************************************************************************
//
//			A standard environment
//			  I am accustomed to
//
// $Id: myenv.h,v 1.4 1995/06/05 16:15:43 oleg Exp oleg $

#ifndef __GNUC__
#pragma once
#endif
#ifndef _myenv_h
#define _myenv_h

#ifdef __GNUC__
#pragma interface
#endif

				/* Strings of symbols			*/
				/* They may be used as a delimiting lines*/
extern const char _Minuses [];
extern const char _Asteriscs [];
extern const char _Equals [];

				/* Print an error message at stderr and	*/
				/* abort				*/
volatile void _error(
	const char * message,		/* Message to be printed	*/
	...                             /* Additional args to printf	*/
	   );

				/* Print a message at stderr 		*/
void message(
	const char * text,		/* Message to be printed	*/
	...                             /* Additional args to printf	*/
	   );

//------------------------------------------------------------------------
//		    Patches to the standard environment

                                // Like strncpy(), but ALWAYS terminates
                                // the destination string
char * xstrncpy(char * dest, const char * src, const int len);

inline long int sqr(const int x)		{ return x*x; }

				// Uncomment if the compiler sucks
#ifdef __MWERKS__
#define bool Boolean
#endif
#if 0
enum bool {false,true};
#endif

				// libg++ nifty timing functions
				// return_elapsed_time(Last_time) returns
				// process time (in secs) since Last_Time
				// If Last_time == 0.0, return time since
				// the last call to start_timer()
#ifndef __GNUC__
double start_timer(void);
double return_elapsed_time(const double Last_Time);
#endif


//------------------------------------------------------------------------
//			Verify the assertion

#if 0
  					/* Print a message and abort*/
extern volatile void _error( const char * message,... ); 
#endif

#define assert(ex) \
        (void)((ex) ? 1 : \
              (_error("Failed assertion " #ex " at line %d of `%s'.\n", \
               __LINE__, __FILE__), 0))
#define assertval(ex) assert(ex)

#define assure(expr,message)				\
	if	(expr) ;				\
	else _error("%s\n at line %d of '%s'.",message,__LINE__, __FILE__)


#endif
