// thread class

// headers
#include "Debug.h"
#include "Thread.h"

namespace ombt {

// base thread ctor and dtor
Thread::Thread():
    BaseObject(true), id_(-1)
{
    ::pthread_attr_init(&attr_);
}

Thread::Thread(const Thread &src):
    BaseObject(src), id_(-1)
{
    ::memcpy((void*)&attr_, (void*)&src.attr_);
}

Thread::~Thread()
{
    if (ok_ && isStarted() && !isDetached())
    {
        void *retval = NULL;
        if (::pthread_cancel(id_) == 0)
        {
            ::pthread_join(id_, &retval);
        }
        ::pthread_attr_destroy(&attr_);
    }
    ok_ = false;
}

// assignment
Thread &
Thread::operator=(const Thread &rhs)
{
    if (this != &rhs)
    {
        ok_ = rhs.ok_;
        id_ = -1;
        ::memcpy((void*)&attr_, (void*)&rhs.attr_);
    }
    return(*this);
}

// get attributes
bool
Thread::isDetached() const
{
    if (!ok_ || !isStarted()) return(false);
    int state;
    ::pthread_attr_getdetachedstate(&attr_, &state);
    if (state == PTHREAD_CREATE_DETACHED)
        return(true);
    else
        return(false);
}

// maintenance
int
Thread::run()
{
    if (!ok_ || isStarted()) return(-1);
    int status = ::pthread_create(&id_, &attr_, start_routine, this);
    if (status != 0) ok_ = false;
    return(status);
}

int
Thread::detach()
{
    if (!ok_ || !isStarted()) return(-1);
    ::pthread_detach(id_);
    ::pthread_attr_setdetachstate(&attr_, PTHREAD_CREATE_DETACHED);
    return(0);
}

int
Thread::cancel()
{
    if (!ok || !isStarted()) return(-1);
    int status = ::pthread_cancel(id_);
    if (status != 0) ok_ = false;
    return(status);
}

int
Thread::join()
{
    if (!ok || !isStarted() || isDetached()) return(-1);
    void *retval = NULL;
    int status = ::pthread_join(id_, &retval);
    if (status != 0) ok_ = false;
    ::pthread_attr_destroy(&attr_);
    return(status);
}

// start-the-thread function
void *
Thread::start_routine(void *arg)
{
    
}

