#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdlib.h>
#include <string.h>
#include <iostream>

#define TRACE() \
	std::cout << "TRACE " << __FUNCTION__ << "'" << __LINE__ << ": " << std::endl
#define DUMP(myvalue) \
	std::cout << "DUMP " << __FUNCTION__ << "'" << __LINE__ << ": " #myvalue " = " << myvalue << std::endl
#define DUMP2(myvalue, myvalue2) \
	std::cout << "DUMP " << __FUNCTION__ << "'" << __LINE__ << ": " #myvalue " = " << myvalue << ", " #myvalue2 " = " << myvalue2 << std::endl

#endif
