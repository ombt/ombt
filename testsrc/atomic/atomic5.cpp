//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "system/Debug.h"
#include "atomic/Atomic.h"
#include "atomic/UseCntPtr.h"
#include "threads/ThreadMutex.h"
#include "threads/ThreadBarrier.h"
#include "logging/Logging.h"
#include "threads/SimpleThread.h"

using namespace std;
using namespace ombt;

class Work: public SimpleThread::Task {
public:
    Work(int ithread): ithread_(ithread) { }
    ~Work() { }
    virtual int init() { return 0; }
    virtual int run() {
        DBGPRINTF("%s", "blocked thread !!!");
        barrier.wait();
        DBGPRINTF("%s", "unblocked thread !!!");
	int i;
	for (i=1; i<=imax; ++i)
	{
            ThreadMutexLock lock(mutex);
            MustBeTrue(atomic.increment_and_test(2));
            MustBeTrue(atomic.decrement_and_test(1));
	}
        dbgprintf("thread %d done (i=%d) !!!\n", ithread_, i);
        return 0;
    }
    virtual int finish() { return 0; }

public:
    static Atomic<> atomic;
    static long imax;
    static ThreadMutex mutex;
    static ThreadBarrier barrier;

private:
    int ithread_;
};

Atomic<> Work::atomic(1);
long Work::imax = 0;
ThreadMutex Work::mutex;
ThreadBarrier Work::barrier(true);

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " numworkers maxtries sleepseconds" << endl;
        return(2);
    }

    int arg=0;
    int maxworkers = ::atoi(argv[++arg]);
    MustBeTrue(maxworkers > 0);
    Work::imax = ::atoi(argv[++arg]);
    MustBeTrue(Work::imax > 0);
    int sleeptime = ::atoi(argv[++arg]);
    MustBeTrue(sleeptime > 0);

    DBGTRACE();
    std::list<UseCntPtr<SimpleThread> > threads;

    DBGPRINTF("%s", "blocking all threads !!!");
    Work::barrier.block();

    DBGTRACE();
    for (int worker=0; worker<maxworkers; worker++)
    {
        DBGTRACE();
        UseCntPtr<Work> pw(new Work(worker+1));
        DBGTRACE();
        UseCntPtr<SimpleThread> pt(new SimpleThread(pw));
        DBGTRACE();
        MustBeTrue(pt->isOk());
        DBGTRACE();
        MustBeTrue(pt->run() == 0);
        DBGTRACE();
        MustBeTrue(pt->isOk());
        DBGTRACE();
        threads.push_back(pt);
    }

    DBGPRINTF("sleep %d seconds !!!", sleeptime);
    ::sleep(sleeptime);

    DBGPRINTF("%s", "unblocking all threads !!!");
    Work::barrier.unblock();

    void *retval;

    std::list<UseCntPtr<SimpleThread> >::iterator tit = threads.begin();
    std::list<UseCntPtr<SimpleThread> >::iterator titend = threads.end();
    for ( ; tit != titend; ++tit)
    {
        DBGTRACE();
        
        MustBeTrue((*tit)->join(retval) == 0);
    }

    return(0);
}

