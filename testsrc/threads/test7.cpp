//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <vector>
#include <Debug.h>
#include <threads/ThreadBarrier.h>
#include <threads/ThreadSafePtr.h>
#include <threads/SimpleThread.h>
#include <logging/Logging.h>

using namespace ombt;
using namespace std;

class ThreadData
{
public:
    ThreadData(): counts_(0) { }
    long counts_;
};

typedef ThreadSafePtr<ThreadData> ThreadDataPtr;

class Worker: public SimpleThread::Task {
public:
    Worker(int id, int imax, ThreadBarrier &tb, ThreadDataPtr &ptd):
        id_(id), imax_(imax), tb_(tb), ptd_(ptd) {
        DBGPRINTF("Worker ctor id: %d", id_);
    }
    ~Worker() {
        DBGPRINTF("Worker dtor id: %d", id_);
    }
    
    virtual int init() {
        DBGPRINTF("Worker init id: %d", id_);
        return 0;
    }
    virtual int run() {
        dbgprintf("Worker run id %d waiting ...\n", id_);
        tb_.wait();
        dbgprintf("Worker run id %d released ...\n", id_);
        for (int i=1; i<=imax_; ++i)
        {
            ptd_->counts_ += 4;
            dbgprintf("Worker run id: %d, count: %d\n", 
                      id_, ptd_->counts_);
        }
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("Worker finish id: %d", id_);
        return 0;
    }

private:
    int id_;
    int imax_;
    ThreadBarrier &tb_;
    ThreadDataPtr &ptd_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " maxcount " << endl;
        return(2);
    }

    int arg=0;
    int imax = ::atoi(argv[++arg]);
    MustBeTrue(imax > 0);

    ThreadData td;
    ThreadDataPtr ptd(&td);

    ThreadBarrier tb;

    SimpleThread st1(new Worker(1, imax, tb, ptd));
    MustBeTrue(st1.isOk());

    MustBeTrue(st1.run() == 0);
    MustBeTrue(st1.isOk());

    SimpleThread st2(new Worker(2, imax, tb, ptd));
    MustBeTrue(st2.isOk());

    MustBeTrue(st2.run() == 0);
    MustBeTrue(st2.isOk());

    tb.unblock();

    void *retval;
    st1.join(retval);
    st2.join(retval);

    return(0);
}
