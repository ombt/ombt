#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <vector>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <threads/ThreadBarrier.h>
#include <threads/ThreadSafePtr.h>
#include <threads/EventQueue.h>
#include <threads/SimpleThread.h>
#include <logging/Logging.h>

using namespace ombt;
using namespace std;

template <class TT>
class ThreadData
{
public:
    ThreadData(int id = 0, TT *thread = NULL):
        id_(id), counts_(0), thread_(thread) { }
    int id_;
    long counts_;
    UseCntPtr<TT> thread_;
};

typedef std::vector<ThreadData<SimpleThread> > ThreadDataGroup;
typedef ThreadSafePtr<ThreadDataGroup> ThreadDataGroupPtr;

class Reader: public SimpleThread::Task {
public:
    Reader(int id, EventQueue<int> &eq, ThreadDataGroupPtr &prts):
        id_(id), eq_(eq), prts_(prts) {
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
            prts_->operator[](event).counts_ += 1;
        }
        return 0;
    }
    virtual int finish() {
        DBGPRINTF("READER FINISH ID: %d", id_);
        return 0;
    }

private:
    int id_;
    EventQueue<int> &eq_;
    ThreadDataGroupPtr &prts_;
};

class Writer: public SimpleThread::Task {
public:
    Writer(int id, int max, ThreadBarrier &tb, EventQueue<int> &eq,
        ThreadDataGroupPtr &pwts):
        id_(id), max_(max), tb_(tb), eq_(eq), pwts_(pwts) {
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
            pwts_->operator[](i).counts_ += 1;
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
    EventQueue<int> &eq_;
    ThreadDataGroupPtr &pwts_;
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

TRACE();
    int arg=0;
    int imax = ::atoi(argv[++arg]);
    MustBeTrue(imax > 0);
    int nwriters = ::atoi(argv[++arg]);
    MustBeTrue(nwriters > 0);
    int nreaders = ::atoi(argv[++arg]);
    MustBeTrue(nreaders > 0);

TRACE();
    EventQueue<int> eq;

TRACE();
    std::list<UseCntPtr<Reader> > readers;
    std::vector<ThreadData<SimpleThread> > readerthreads;
    readerthreads.resize(nreaders+1);
    ThreadSafePtr<std::vector<ThreadData<SimpleThread> > > prts(&readerthreads);

TRACE();
    for (int ireader=1; ireader<=nreaders; ++ireader)
    {
        UseCntPtr<Reader> reader(new Reader(ireader, eq, prts));
	readers.push_back(reader);

        UseCntPtr<SimpleThread> st(new SimpleThread(reader));
        MustBeTrue(st->isOk());

        MustBeTrue(st->run() == 0);
        MustBeTrue(st->isOk());

	readerthreads[ireader] = ThreadData<SimpleThread>(ireader, st);
    }

TRACE();
    ThreadBarrier tb(true);

TRACE();
    std::list<UseCntPtr<Writer> > writers;
    std::vector<ThreadData<SimpleThread> > writerthreads;
    writerthreads.resize(nwriters+1);
    ThreadSafePtr<std::vector<ThreadData<SimpleThread> > > pwts(&writerthreads);

TRACE();
    for (int iwriter=1; iwriter<=nwriters; ++iwriter)
    {
        UseCntPtr<Writer> writer(new Writer(iwriter, imax, tb, eq, pwts));
	writers.push_back(writer);
TRACE();

        UseCntPtr<SimpleThread> st(new SimpleThread(writer));
        MustBeTrue(st->isOk());
TRACE();

        MustBeTrue(st->run() == 0);
        MustBeTrue(st->isOk());
TRACE();

	writerthreads[iwriter] = ThreadData<SimpleThread>(iwriter, st);
TRACE();
    }
TRACE();

    tb.unblock();
TRACE();

    void *retval;

TRACE();
    std::vector<ThreadData<SimpleThread> >::iterator writer = writerthreads.begin();
    std::vector<ThreadData<SimpleThread> >::iterator writerend = writerthreads.end();
    for (++writer; writer != writerend; ++writer)
    {
TRACE();
        dbgprintf("JOINING TO WRITER ID: %d\n", writer->id_);
TRACE();
        writer->thread_->join(retval);
TRACE();
        dbgprintf("JOINED TO WRITER ID: %d\n", writer->id_);
TRACE();
    }

TRACE();
    eq.done();

TRACE();
    std::vector<ThreadData<SimpleThread> >::iterator reader = readerthreads.begin();
    std::vector<ThreadData<SimpleThread> >::iterator readerend = readerthreads.end();
    for (++reader; reader != readerend; ++reader)
    {
TRACE();
        dbgprintf("JOINING TO READER ID: %d\n", reader->id_);
        reader->thread_->join(retval);
        dbgprintf("JOINED TO READER ID: %d\n", reader->id_);
TRACE();
    }

TRACE();
    return(0);
}
