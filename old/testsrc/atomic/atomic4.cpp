
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <Debug.h>
#include <atomic/Atomic.h>
#include <atomic/UseCntPtr.h>
#include <threads/ThreadMutex.h>
#include <threads/ThreadBarrier.h>
#include <logging/Logging.h>
#include <threads/SimpleThread.h>

using namespace std;
using namespace ombt;

static bool spin = true;
static Atomic<> atomic(1);
static long imax = 0;
static ThreadMutex mutex;
static ThreadBarrier barrier(true);

class Work: public SimpleThread::Task {
public:
    virtual int init() { return 0; }
    virtual int run() {
        DBGPRINTF("%s", "blocked thread !!!");
        barrier.wait();
        DBGPRINTF("%s", "unblocked thread !!!");
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

    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " numworkers maxtries sleepseconds" << endl;
        return(2);
    }

    int arg=0;
    int maxworkers = ::atoi(argv[++arg]);
    MustBeTrue(maxworkers > 0);
    imax = ::atoi(argv[++arg]);
    MustBeTrue(imax > 0);
    int sleeptime = ::atoi(argv[++arg]);
    MustBeTrue(sleeptime > 0);

    DBGTRACE();
    
    std::list<UseCntPtr<SimpleThread> > threads;

    DBGPRINTF("%s", "blocking all threads !!!");
    barrier.block();

    for (int worker=0; worker<maxworkers; worker++)
    {
        UseCntPtr<SimpleThread> pt(new SimpleThread(new Work));
        MustBeTrue(pt->isOk());
        MustBeTrue(pt->run() == 0);
        MustBeTrue(pt->isOk());
        threads.push_back(pt);
    }

    DBGPRINTF("sleep %d seconds !!!", sleeptime);
    ::sleep(sleeptime);

    DBGPRINTF("%s", "unblocking all threads !!!");
    barrier.unblock();

    void *retval;

    std::list<UseCntPtr<SimpleThread> >::iterator sti = threads.begin();
    std::list<UseCntPtr<SimpleThread> >::iterator stiend = threads.end();
    for ( ; sti != stiend; ++sti)
    {
        DBGTRACE();
        MustBeTrue((*sti)->join(retval) == 0);
    }

    return(0);
}

