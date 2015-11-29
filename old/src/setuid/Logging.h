#ifndef __OMBT_LOGGING_H
#define __OMBT_LOGGING_H
// logging class

// headers
#include <stdio.h>

// useful macros
#define LOGTRACE() \
	Logging::logger->printf("%s'%s'%d\n", __FILE__,__FUNCTION__,__LINE__);

// headers 
class Logging
{
public:
    // logger
    static Logging *logger;
    static FILE *loggerfd;

    // ctor and dtor
    Logging(bool on = false);
    virtual Logging();

    // format function
    printf(const char *fmt, ...);
};

#endif
