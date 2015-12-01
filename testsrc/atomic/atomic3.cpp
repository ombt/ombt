//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "system/Debug.h"
#include "atomic/Atomic.h"
#include "atomic/UseCntPtr.h"
#include "threads/ThreadMutex.h"
#include "logging/Logging.h"
#include "threads/SimpleThread.h"

using namespace std;
using namespace ombt;

static bool spin = true;
static Atomic<> atomic(1);
static long imax = 0;
static ThreadMutex mutex;

class Work: public SimpleThread::Task {
public:
    virtual int init() { return 0; }
    virtual int run() {
        DBGTRACE();
        while (spin);
        DBGTRACE();
	for (int i=1; i<=imax; ++i)
	{
            ThreadMutexLock lock(mutex);
            MustBeTrue(atomic.increment_and_test(2));
            MustBeTrue(atomic.decrement_and_test(1));
	}
        DBGTRACE();
        return 0;
    }
    virtual int finish() { return 0; }
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " numworkers maxtries" << endl;
        return(2);
    }

    int arg=0;
    int maxworkers = ::atoi(argv[++arg]);
    MustBeTrue(maxworkers > 0);
    imax = ::atoi(argv[++arg]);
    MustBeTrue(imax > 0);

    DBGTRACE();
    std::vector<UseCntPtr<SimpleThread> > threads;
    threads.resize(maxworkers);

    DBGTRACE();
    for (int worker=0; worker<maxworkers; worker++)
    {
        DBGTRACE();
        threads[worker] = new SimpleThread(new Work);
        DBGTRACE();
        MustBeTrue(threads[worker]->isOk());
        DBGTRACE();
        MustBeTrue(threads[worker]->run() == 0);
        DBGTRACE();
        MustBeTrue(threads[worker]->isOk());
        DBGTRACE();
    }

    DBGTRACE();
    spin = false;

    void *retval;

    for (int worker=0; worker<maxworkers; worker++)
    {
        DBGTRACE();
        
        MustBeTrue(threads[worker]->join(retval) == 0);
    }

    return(0);
}

