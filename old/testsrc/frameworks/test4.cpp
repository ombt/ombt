#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include <Debug.h>
#include <logging/Logging.h>

#include <atomic/UseCntPtr.h>
#include <frameworks/Queue.h>
#include <frameworks/Task.h>
#include <frameworks/Thread.h>

using namespace ombt;
using namespace std;

static bool spin = true;

class Reader: public Task {
public:
    Reader(int id, Queue<int> &eq):
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
        int event;
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
    Queue<int> &eq_;
};

class Writer: public Task {
public:
    Writer(int id, int max, Queue<int> &eq):
        id_(id), max_(max), eq_(eq) { }

    virtual int init() {
        return 0;
    }
    virtual int run() {
        for (int i=1; i<=max_; ++i)
        {
            eq_.enqueue(i);
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
        UseCntPtr<Reader> reader(new Reader(ireader, eq));
	readers.push_back(reader);

        UseCntPtr<Thread> st(new Thread(reader));
        MustBeTrue(st->isOk());

        MustBeTrue(st->run() == 0);
        MustBeTrue(st->isOk());

	threads.push_back(st);
    }

    UseCntPtr<Thread> st(new Thread(new Writer(0, imax, eq)));
    MustBeTrue(st->isOk());

    MustBeTrue(st->run() == 0);
    MustBeTrue(st->isOk());

    void *retval;
    st->join(retval);

    eq.done();

    std::list<UseCntPtr<Thread> >::iterator iter = threads.begin();
    std::list<UseCntPtr<Thread> >::iterator iterend = threads.end();
    for ( ; iter != iterend; ++iter)
    {
        (*iter)->join(retval);
    }

    return(0);
}
