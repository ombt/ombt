#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <threads/SimpleThread.h>
#include <threads/ThreadMutex.h>
#include <threads/ThreadConditionVariable.h>
#include <logging/Logging.h>

using namespace ombt;
using namespace std;

template <class EventType>
class EventQueue {
public:
    // ctor and dtor
    EventQueue(): 
        done_(false), events_(), pcv_(), pcv_mutex_() {
        pcv_mutex_ = new ThreadMutex;
        MustBeTrue(pcv_mutex_ != NULL);
        pcv_ = new ThreadConditionVariable();
        MustBeTrue(pcv_ != NULL);
    }
    ~EventQueue() { }

    // accessing the queue
    void enqueue(EventType &event) {
        pcv_mutex_->lock();
        if (events_.size() <= maxsize_)
            events_.push_back(event);
        pcv_->signal();
        pcv_mutex_->unlock();
    }
    bool dequeue(EventType &event) {
        pcv_mutex_->lock();
        while (events_.empty())
        {
            if (done_)
            {
                pcv_->signal();
                pcv_mutex_->unlock();
                return false;
            }
            pcv_->wait(pcv_mutex_);
        }
        event = events_.front();
        events_.pop_front();
        pcv_->signal();
        pcv_mutex_->unlock();
        return true;
    }
    void done() {
        pcv_mutex_->lock();
        done_ = true;
        pcv_->broadcast();
        pcv_mutex_->unlock();
    }

protected:
    // data
    typedef std::list<EventType> Queue;

    bool done_;
    int maxsize_;
    Queue events_;
    UseCntPtr<ThreadConditionVariable> pcv_;
    UseCntPtr<ThreadMutex> pcv_mutex_;
};

class Reader: public SimpleThread::Task {
public:
    Reader(int id, EventQueue<int> &eq):
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
    EventQueue<int> &eq_;
};

class Writer: public SimpleThread::Task {
public:
    Writer(int id, int max, EventQueue<int> &eq):
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
        //::usleep(1000*1000);
        return 0;
    }
    virtual int finish() {
        return 0;
    }

private:
    int id_;
    int max_;
    EventQueue<int> &eq_;
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

    EventQueue<int> eq;

    std::list<UseCntPtr<Reader> > readers;
    std::list<UseCntPtr<SimpleThread> > threads;

    for (int ireader=1; ireader<=nreaders; ++ireader)
    {
        UseCntPtr<Reader> reader(new Reader(ireader, eq));
	readers.push_back(reader);

        UseCntPtr<SimpleThread> st(new SimpleThread(reader));
        MustBeTrue(st->isOk());

        MustBeTrue(st->run() == 0);
        MustBeTrue(st->isOk());

	threads.push_back(st);
    }

    SimpleThread st(new Writer(0, imax, eq));
    MustBeTrue(st.isOk());

    MustBeTrue(st.run() == 0);
    MustBeTrue(st.isOk());

    void *retval;
    st.join(retval);

    eq.done();

    std::list<UseCntPtr<SimpleThread> >::iterator iter = threads.begin();
    std::list<UseCntPtr<SimpleThread> >::iterator iterend = threads.end();
    for ( ; iter != iterend; ++iter)
    {
        (*iter)->join(retval);
    }

    return(0);
}
