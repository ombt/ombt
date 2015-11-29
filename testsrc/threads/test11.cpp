//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <list>

#include <Debug.h>
#include <logging/Logging.h>
#include <random/Random.h>

#include <atomic/UseCntPtr.h>
#include <threads/PriorityQueue.h>
#include <threads/Task.h>
#include <threads/Thread.h>

using namespace ombt;
using namespace std;

static bool spin = true;

class Reader: public Task {
public:
    Reader(int id, PriorityQueue<unsigned long> &eq):
        id_(id), eq_(eq) {
        DBGPRINTF("DTOR ID: %d", id_);
    }
    ~Reader() {
        DBGPRINTF("DTOR ID: %d", id_);
    }
    
    virtual int init() {
        DBGPRINTF("INIT ID: %d", id_);
        return 0;
    }
    virtual int run() {
        DBGPRINTF("RUN ID: %d", id_);
        unsigned long event;
        while (eq_.dequeue(event))
        {
            dbgprintf("ID: %d, EVENT: %d\n", id_, event);
            if (event < 0) break;
        }
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("FINISH ID: %d", id_);
        return 0;
    }

private:
    int id_;
    PriorityQueue<unsigned long> &eq_;
};

class Writer: public Task {
public:
    Writer(int id, int max, PriorityQueue<unsigned long> &eq):
        id_(id), max_(max), eq_(eq) { }

    virtual int init() {
        return 0;
    }
    virtual int run() {
        for (int i=1; i<=max_; ++i)
        {
            unsigned long tmp = myrandom()%1000;
            eq_.enqueue(tmp);
            //::usleep(100*1000);
        }
        return 0;
    }
    virtual int finish() {
        return 0;
    }

private:
    int id_;
    int max_;
    PriorityQueue<unsigned long> &eq_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " maxcount readernum" << endl;
        return(2);
    }

    int arg=0;
    int imax = ::atoi(argv[++arg]);
    MustBeTrue(imax > 0);
    int nreaders = ::atoi(argv[++arg]);
    MustBeTrue(nreaders > 0);

    setKey(time(NULL)|1);

    PriorityQueue<unsigned long> eq;

    std::list<UseCntPtr<Thread> > threads;

    for (int ireader=1; ireader<=nreaders; ++ireader)
    {
        UseCntPtr<Thread> pt(new Thread(new Reader(ireader, eq)));
        MustBeTrue(pt->isOk());
        MustBeTrue(pt->run() == 0);
        MustBeTrue(pt->isOk());

	threads.push_back(pt);
    }

    Thread wt(new  Writer(0, imax, eq));

    MustBeTrue(wt.isOk());
    MustBeTrue(wt.run() == 0);
    MustBeTrue(wt.isOk());

    void *retval;
    wt.join(retval);

    eq.done();

    std::list<UseCntPtr<Thread > >::iterator iter = threads.begin();
    std::list<UseCntPtr<Thread > >::iterator iterend = threads.end();
    for ( ; iter != iterend; ++iter)
    {
        (*iter)->join(retval);
    }

    return(0);
}
