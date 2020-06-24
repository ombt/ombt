#ifndef __DEBUG_H
#define __DEBUG_H
#define TRACE() print *, __FILE__, '''', __LINE__, ' '
#define PRINT TRACE(),
#endif
