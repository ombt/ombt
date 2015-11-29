#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <list>
#include <vector>

#include <Debug.h>
#include <logging/Logging.h>

#include <frameworks/IterativeServer.h>

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
    dbgopen(stderr);

DBGPRINTF("%s", "before IterativeServer ctor");
    IterativeServer is;
DBGPRINTF("%s", "after IterativeServer ctor");

DBGPRINTF("%s", "before IterativeServer init");
    MustBeTrue(is.init() == 0);
DBGPRINTF("%s", "after IterativeServer init");

DBGPRINTF("%s", "before IterativeServer run");
    MustBeTrue(is.run() == 0);
DBGPRINTF("%s", "after IterativeServer run");

DBGPRINTF("%s", "before IterativeServer finish");
    MustBeTrue(is.finish() == 0);
DBGPRINTF("%s", "after IterativeServer finish");

    return(0);
}
