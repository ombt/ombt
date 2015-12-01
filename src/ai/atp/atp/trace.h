#ifndef __TRACE_H
#define __TRACE_H
// trace execution

// headers
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>
#include <adt/Map.h>
#include <adt/Twokeys.h>

using namespace std;
using namespace ombt;

// externs definitions
extern Map<unsigned long, long> trace1;
extern Map<TwoKeys<unsigned long, unsigned long>, long> trace2;
extern unsigned long initialedata;
extern unsigned long finaledata;
extern unsigned long previousfinaledata;

// definitions
#define DEFINETRACE1() \
	Map<unsigned long, long> trace1

#define DEFINETRACE2() \
	Map<TwoKeys<unsigned long, unsigned long>, long> trace2

#define INCRTRACE1(_ul1) { \
	if (!trace1.isInMap((unsigned long)_ul1)) \
		trace1[(unsigned long)_ul1] = 0; \
	trace1[(unsigned long)_ul1] += 1; \
}

#define DECRTRACE1(_ul1) { \
	if (!trace1.isInMap((unsigned long)_ul1)) \
		trace1[(unsigned long)_ul1] = 0; \
	trace1[(unsigned long)_ul1] -= 1; \
}

#define INCRTRACE2(_ul1, _ul2) { \
	TwoKeys<unsigned long, unsigned long> \
		twokeys((unsigned long)_ul1, (unsigned long)_ul2); \
	if (!trace2.isInMap(twokeys)) \
		trace2[twokeys] = 0; \
	trace2[twokeys] += 1; \
}

#define DECRTRACE2(_ul1, _ul2) { \
	TwoKeys<unsigned long, unsigned long> \
		twokeys((unsigned long)_ul1, (unsigned long)_ul2); \
	if (!trace2.isInMap(twokeys)) \
		trace2[twokeys] = 0; \
	trace2[twokeys] -= 1; \
}

#define DUMPTRACE1(_ostr) \
	_ostr << endl << "TRACE (1) DUMP at " << __FILE__ << "'" << __LINE__ << ": " << trace1 << endl;

#define DUMPTRACE2(_ostr) \
	_ostr << endl << "TRACE (2) DUMP at " << __FILE__ << "'" << __LINE__ << ": " << trace2 << endl;

#define DUMPTRACE(_ostr) { \
	DUMPTRACE1(_ostr); \
	DUMPTRACE2(_ostr); \
}
	
#define DEFINEDATA() \
	unsigned long initialedata = 0; \
	unsigned long finaledata = 0; \
	unsigned long previousfinaledata = 0; \

#define SETINITEDATA() \
	initialedata = (unsigned long)sbrk(0)

#define SETFINALEDATA() { \
	previousfinaledata = finaledata; \
	finaledata = (unsigned long)sbrk(0); \
}

#define DUMPDATA(_ostr) { \
	_ostr << endl; \
	_ostr << "Dump data at " << __FILE__ << "'" << __LINE__ << ": " << endl; \
	_ostr << "Initial Data Address: " << initialedata << endl; \
	_ostr << "Final Data Address: " << finaledata << endl; \
	_ostr << "Growth in Data Section: " << (finaledata-initialedata) << endl; \
	if ((finaledata-previousfinaledata) > 0) \
	{ \
		_ostr << "Delta in Data Section: " << (finaledata-previousfinaledata) << endl; \
	} \
}

#endif
