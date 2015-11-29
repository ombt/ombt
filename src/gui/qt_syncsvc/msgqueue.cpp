// headers
#include "msgqueue.h"

MsgQueue::MsgQueue(): done_(false) { }
MsgQueue::~MsgQueue() { done_ = true; }

void 
MsgQueue::enqueue(const QString &msg)
{
    mutex_.lock();
    if (done_)
    {
        mutex_.unlock();
        condvar_.wakeOne();
        return;
    }
    msgqueue_.push_back(msg);
    mutex_.unlock();
    condvar_.wakeOne();
}

bool 
MsgQueue::dequeue(QString &msg)
{
    mutex_.lock();
    while (msgqueue_.empty())
    {
        if (done_)
        {
            mutex_.unlock();
            condvar_.wakeOne();
            return(false);
        }
        condvar_.wait(&mutex_);
    }
    msg = msgqueue_.front();
    msgqueue_.pop_front();
    mutex_.unlock();
    condvar_.wakeOne();
    return(true);
}

bool 
MsgQueue::head(QString &msg)
{
    mutex_.lock();
    while (msgqueue_.empty())
    {
        if (done_)
        {
            mutex_.unlock();
            condvar_.wakeOne();
            return(false);
        }
        condvar_.wait(&mutex_);
    }
    msg = msgqueue_.front();
    mutex_.unlock();
    condvar_.wakeOne();
    return(true);
}

void 
MsgQueue::done()
{
    mutex_.lock();
    done_ = true;
    mutex_.unlock();
    condvar_.wakeAll();
}
