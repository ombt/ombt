//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include <Debug.h>
#include <logging/Logging.h>

#include <atomic/UseCntPtr.h>
#include <threads/Task.h>
#include <threads/Thread.h>
#include <threads/Queue.h>

using namespace ombt;
using namespace std;

static bool spin = true;

class Reader: public Task {
public:
    Reader(int id, Queue<int> &eq):
        id_(id), eq_(eq) {
        DBGPRINTF("Reader: CTOR ID: %d", id_);
    }
    ~Reader() {
        DBGPRINTF("Reader: DTOR ID: %d", id_);
    }
    
    virtual int init() {
        DBGPRINTF("Reader: INIT ID: %d", id_);
        return 0;
    }
    virtual int run() {
        DBGPRINTF("Reader: RUN ID: %d", id_);
        int event;
        //DBGPRINTF("Reader: RUN ID: %d", id_);
        while (eq_.dequeue(event))
        {
            //DBGPRINTF("Reader: RUN ID: %d", id_);
            dbgprintf("Reader: ID: %d, EVENT: %d\n", id_, event);
            //DBGPRINTF("Reader: RUN ID: %d", id_);
            if (event < 0) break;
            //DBGPRINTF("Reader: RUN ID: %d", id_);
        }
        //DBGPRINTF("Reader: RUN ID: %d", id_);
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("Reader: FINISH ID: %d", id_);
        return 0;
    }

private:
    int id_;
    Queue<int> &eq_;
};

class Writer: public Task {
public:
    Writer(int id, int max, Queue<int> &eq):
        id_(id), max_(max), eq_(eq) {
        DBGPRINTF("Writer: CTOR ID: %d", id_);
    }
    ~Writer() {
        DBGPRINTF("Writer: DTOR ID: %d", id_);
    }

    virtual int init() {
        DBGPRINTF("Writer: INIT ID: %d", id_);
        return 0;
    }
    virtual int run() {
        DBGPRINTF("Writer: RUN ID: %d", id_);
        for (int i=1; i<=max_; ++i)
        {
            //DBGPRINTF("Writer: RUN ID: %d", id_);
            eq_.enqueue(i);
            //::usleep(100*1000);
        }
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("Writer: FINISH ID: %d", id_);
        return 0;
    }

private:
    int id_;
    int max_;
    Queue<int> &eq_;
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

    Queue<int> eq;

    std::list<UseCntPtr<Reader> > readers;
    std::list<UseCntPtr<Thread> > threads;

    for (int ireader=1; ireader<=nreaders; ++ireader)
    {
        DBGPRINTF("STARTING READER: ID: %d", ireader);
        UseCntPtr<Reader> reader(new Reader(ireader, eq));
	readers.push_back(reader);

        UseCntPtr<Thread> st(new Thread(reader));
        MustBeTrue(st->isOk());

        MustBeTrue(st->run() == 0);
        MustBeTrue(st->isOk());

	threads.push_back(st);
    }

    DBGPRINTF("STARTING READER: ID: %d", 1);
    Thread st(new Writer(1, imax, eq));
    MustBeTrue(st.isOk());

    MustBeTrue(st.run() == 0);
    MustBeTrue(st.isOk());

    void *retval;
    st.join(retval);

    eq.done();

    std::list<UseCntPtr<Thread> >::iterator iter = threads.begin();
    std::list<UseCntPtr<Thread> >::iterator iterend = threads.end();
    for ( ; iter != iterend; ++iter)
    {
        (*iter)->join(retval);
    }

    return(0);
}
