//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <threads/ThreadBarrier.h>
#include <logging/Logging.h>

#include <threads/Queue.h>
#include <threads/Thread.h>
#include <threads/Task.h>

using namespace ombt;
using namespace std;

static bool spin = true;

class Reader: public Task {
public:
    Reader(int id, Queue<int> &eq):
        id_(id), eq_(eq) {
        DBGPRINTF("READER CTOR ID: %d", id_);
    }
    ~Reader() {
        DBGPRINTF("READER DTOR ID: %d", id_);
    }
    
    virtual int init() {
        DBGPRINTF("READER INIT ID: %d", id_);
        return 0;
    }
    virtual int run() {
        DBGPRINTF("READER RUN ID: %d", id_);
        int event;
        while (eq_.dequeue(event))
        {
            dbgprintf("READER ID: %d, EVENT: %d\n", id_, event);
            if (event < 0) break;
        }
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("READER FINISH ID: %d", id_);
        return 0;
    }

private:
    int id_;
    Queue<int> &eq_;
};

class Writer: public Task {
public:
    Writer(int id, int max, ThreadBarrier &tb, Queue<int> &eq):
        id_(id), max_(max), tb_(tb), eq_(eq) {
        DBGPRINTF("WRITER CTOR ID: %d", id_);
    }
    ~Writer() {
        DBGPRINTF("WRITER DTOR ID: %d", id_);
    }

    virtual int init() {
        DBGPRINTF("WRITER INIT ID: %d", id_);
        return 0;
    }
    virtual int run() {
        DBGPRINTF("WRITER RUN ID %d - BLOCKED", id_);
        tb_.wait();
        DBGPRINTF("WRITER RUN ID %d - UNBLOCKED", id_);
        for (int i=1; i<=max_; ++i)
        {
            dbgprintf("WRITER ID: %d, ENQUEUE EVENT: %d\n", id_, i);
            eq_.enqueue(i);
        }
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("WRITER FINISH ID: %d", id_);
        return 0;
    }

private:
    int id_;
    int max_;
    ThreadBarrier &tb_;
    Queue<int> &eq_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " maxcount nwriters nreaders" << endl;
        return(2);
    }

    int arg=0;
    int imax = ::atoi(argv[++arg]);
    MustBeTrue(imax > 0);
    int nwriters = ::atoi(argv[++arg]);
    MustBeTrue(nwriters > 0);
    int nreaders = ::atoi(argv[++arg]);
    MustBeTrue(nreaders > 0);

    Queue<int> eq;

    std::list<UseCntPtr<Thread> > readerthreads;

    for (int ireader=1; ireader<=nreaders; ++ireader)
    {
        UseCntPtr<Thread> pt(new Thread(new Reader(ireader, eq)));
        MustBeTrue(pt->isOk());
        MustBeTrue(pt->run() == 0);
        MustBeTrue(pt->isOk());

	readerthreads.push_back(pt);
    }

    ThreadBarrier tb(true);

    std::list<UseCntPtr<Thread> > writerthreads;

    for (int iwriter=1; iwriter<=nwriters; ++iwriter)
    {
        UseCntPtr<Thread> pt(new Thread(new Writer(iwriter, imax, tb, eq)));
        MustBeTrue(pt->isOk());
        MustBeTrue(pt->run() == 0);
        MustBeTrue(pt->isOk());

	writerthreads.push_back(pt);
    }

    tb.unblock();

    void *retval;

    std::list<UseCntPtr<Thread> >::iterator writer = writerthreads.begin();
    std::list<UseCntPtr<Thread> >::iterator writerend = writerthreads.end();
    for ( ; writer != writerend; ++writer)
    {
        dbgprintf("JOINING TO WRITER ID: %lu\n", (*writer)->getHandle());
        (*writer)->join(retval);
        dbgprintf("JOINED TO WRITER ID: %lu\n", (*writer)->getHandle());
    }

    eq.done();

    std::list<UseCntPtr<Thread> >::iterator reader = readerthreads.begin();
    std::list<UseCntPtr<Thread> >::iterator readerend = readerthreads.end();
    for ( ; reader != readerend; ++reader)
    {
        dbgprintf("JOINING TO READER ID: %lu\n", (*reader)->getHandle());
        (*reader)->join(retval);
        dbgprintf("JOINED TO READER ID: %lu\n", (*reader)->getHandle());
    }

    return(0);
}
