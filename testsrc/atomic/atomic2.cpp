//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "system/Debug.h"
#include "atomic/Atomic.h"
#include "logging/Logging.h"
#include "threads/SimpleThread.h"

using namespace std;
using namespace ombt;

// this test is *suppose* to fail when using the stub functions
// since there is *no* atomic operations, just plain arithmetic.
// once the real atomic ops are in, then this test should NEVER
// fail.

static bool spin = true;
static Atomic<unsigned long> atomic(1);
static long imax = 0;

class First: public SimpleThread::Task {
public:
    virtual int init() { return 0; }
    virtual int run() {
        DBGTRACE();
        while (spin);
        DBGTRACE();
	for (int i=1; i<=imax; ++i)
	{
            MustBeTrue(atomic.increment_and_test(2));
            MustBeTrue(atomic.decrement_and_test(1));
	}
        return 0;
    }
    virtual int finish() { return 0; }
};

class Second: public SimpleThread::Task {
public:
    virtual int init() { return 0; }
    virtual int run() {
        DBGTRACE();
        while (spin);
        DBGTRACE();
	for (int i=1; i<=imax; ++i)
	{
            MustBeTrue(atomic.increment_and_test(2));
            MustBeTrue(atomic.decrement_and_test(1));
	}
        return 0;
    }
    virtual int finish() { return 0; }
};
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    MustBeTrue(argc == 2);

    imax = ::atoi(argv[1]);
    MustBeTrue(imax > 0);

    DBGTRACE();
    SimpleThread st(new First);
    MustBeTrue(st.isOk());

    DBGTRACE();
    MustBeTrue(st.run() == 0);
    MustBeTrue(st.isOk());

    DBGTRACE();
    SimpleThread st2(new Second);
    MustBeTrue(st2.isOk());

    DBGTRACE();
    MustBeTrue(st2.run() == 0);
    MustBeTrue(st2.isOk());

    DBGTRACE();
    spin = false;

    void *retval;

    DBGTRACE();
    st.join(retval);
    MustBeTrue(!st.isOk());

    DBGTRACE();
    st2.join(retval);
    MustBeTrue(!st2.isOk());

    DBGTRACE();

    return(0);
}

