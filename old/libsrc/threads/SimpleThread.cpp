// thread class

// headers
#include <Debug.h>
#include <logging/Logging.h>
#include <SimpleThread.h>

namespace ombt {

// simple thread class
void *
SimpleThread::execute(void *data)
{
    SimpleThread *pthread = static_cast<SimpleThread * >(data);
    MustBeTrue(pthread != NULL);
    return(pthread->dispatch());
}

void *
SimpleThread::dispatch()
{
    int status;
    status = ptask_->init();
    if (status != 0) return((void*)status);
    status = ptask_->run();
    if (status != 0) return((void*)status);
    return((void*)ptask_->finish());
}

// ctor and dtor
SimpleThread::SimpleThread(Task *ptask):
    BaseObject(), ptask_(ptask), id_(-1)
{
    ::pthread_attr_init(&attr_);
    ok_ = true;
}

SimpleThread::~SimpleThread()
{
    if (ok_ && isStarted() && !isDetached())
    {
        void *retval = NULL;
        if (::pthread_cancel(id_) == 0)
        {
            ::pthread_join(id_, &retval);
        }
    }
    ::pthread_attr_destroy(&attr_);
    ok_ = false;
}

// set/get attributes
bool
SimpleThread::isDetached() const
{
    int state;
    ::pthread_attr_getdetachstate(&attr_, &state);
    return(state == PTHREAD_CREATE_DETACHED);
}

bool
SimpleThread::isJoinable() const
{
    int state;
    ::pthread_attr_getdetachstate(&attr_, &state);
    return(state == PTHREAD_CREATE_JOINABLE);
}

void
SimpleThread::setDetachedAttr(bool flag)
{
    if (!ok_ || isStarted()) return;

    if (flag)
        ::pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_DETACHED);
    else
        ::pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_JOINABLE);
}

// maintenance
int
SimpleThread::detach()
{
    if (!ok_ || !isStarted()) return(-1);
    ::pthread_detach(id_);
    ::pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_DETACHED);
    return(0);
}

int
SimpleThread::join(void *&retval)
{
    if (!ok_ || !isStarted() || isDetached()) return(-1);
    retval = NULL;
    int status = ::pthread_join(id_, &retval);
    ok_ = false;
    return(status);
}

int
SimpleThread::cancel()
{
    if (!ok_ || !isStarted() || isDetached()) return(-1);
    int status = ::pthread_cancel(id_);
    ok_ = (status == 0);
    return(status);
}


int
SimpleThread::run()
{
    if (!ok_ || isStarted()) return(-1);
    int status = ::pthread_create(&id_, &attr_, execute, this);
    ok_ = (status == 0);
    return(status);
}

}
