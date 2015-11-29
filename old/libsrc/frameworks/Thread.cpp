// thread class

// headers
#include <Debug.h>
#include <logging/Logging.h>
#include <Task.h>
#include <Thread.h>
#include <threads/ThreadMutex.h>

namespace ombt {

// generate next id
unsigned long 
Thread::nextHandle()
{
    static ThreadMutex mutex;
    static unsigned long nextHandleToUse = 0;
    ThreadMutexLock lock(mutex);
    return(++nextHandleToUse);
}

// thread class
void *
Thread::execute(void *data)
{
    Thread *pthread = static_cast<Thread * >(data);
    MustBeTrue(pthread != NULL);
    return(pthread->dispatch());
}


void *
Thread::dispatch()
{
    MustBeTrue(ptask_ != NULL);
    int status = ptask_->init();
    if (status == 0)
    {
        status = ptask_->run();
        if (status == 0)
        {
            status = ptask_->finish();
        }
    }
    ThreadMutexLock lock(state_.cs_);
    state_.canceled_ = true;
    return((void*)status);
}

// ctor and dtor
Thread::Thread(Task *ptask):
    BaseObject(), handle_(nextHandle()), ptask_(ptask)
{
    MustBeTrue(ptask_ != NULL);
    ::pthread_attr_init(&state_.attr_);
    ok_ = true;
}

Thread::~Thread()
{
    ThreadMutexLock lock(state_.cs_);

    if (ok_ && isStarted() && !isDetached())
    {
        void *retval = NULL;
        if (!isCanceled())
        {
            state_.canceled_ = true;
            ::pthread_cancel(state_.id_);
        }
        if (!isJoined())
        {
            state_.joined_ = true;
            ::pthread_join(state_.id_, &retval);
        }
    }
    ::pthread_attr_destroy(&state_.attr_);
    ok_ = false;
}

// set/get attributes
unsigned long
Thread::getHandle() const
{
    return handle_;
}

bool
Thread::isDetached() const
{
    int state;
    ::pthread_attr_getdetachstate(&state_.attr_, &state);
    return(state == PTHREAD_CREATE_DETACHED);
}

bool
Thread::isJoined() const
{
    return(state_.joined_);
}

bool
Thread::isCanceled() const
{
    return(state_.canceled_);
}

bool
Thread::isJoinable() const
{
    int state;
    ::pthread_attr_getdetachstate(&state_.attr_, &state);
    return(state == PTHREAD_CREATE_JOINABLE);
}

bool 
Thread::isStarted() const
{
    return(state_.started_);
}

void
Thread::setDetachedAttr(bool flag)
{
    ThreadMutexLock lock(state_.cs_);
    if (!ok_ || isStarted()) return;
    if (flag)
        ::pthread_attr_setdetachstate(&state_.attr_, PTHREAD_CREATE_DETACHED);
    else
        ::pthread_attr_setdetachstate(&state_.attr_, PTHREAD_CREATE_JOINABLE);
}

// maintenance
int
Thread::detach()
{
    if (!ok_ || !isStarted()) return(-1);
    ThreadMutexLock lock(state_.cs_);
    ::pthread_detach(state_.id_);
    ::pthread_attr_setdetachstate(&state_.attr_, PTHREAD_CREATE_DETACHED);
    return(0);
}

int
Thread::join(void *&retval)
{
    if (!ok_ || !isStarted() || isDetached()) return(-1);
    if (isJoined()) return(0);
    retval = NULL;
    int status = ::pthread_join(state_.id_, &retval);
    ThreadMutexLock lock(state_.cs_);
    state_.joined_ = true;
    ok_ = (status == 0);
    return(status);
}

int
Thread::cancel()
{
    if (!ok_ || !isStarted() || isDetached()) return(-1);
    if (isCanceled()) return(0);
    int status = ::pthread_cancel(state_.id_);
    ThreadMutexLock lock(state_.cs_);
    state_.canceled_ = true;
    ok_ = (status == 0);
    return(status);
}

int
Thread::run()
{
    if (!ok_ || isStarted()) return(-1);
    state_.started_ = true;
    int status = ::pthread_create(&state_.id_, &state_.attr_, execute, this);
    ok_ = (status == 0);
    return(status);
}

}
