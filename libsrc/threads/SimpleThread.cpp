//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// thread class

// headers
#include <Returns.h>
#include <Debug.h>
#include <SimpleThread.h>
#include <ThreadMutex.h>
#include <logging/Logging.h>

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
    UCBaseObject(OK), ptask_(ptask), state_()
{
    ::pthread_attr_init(&state_.attr_);
}

SimpleThread::~SimpleThread()
{
    ThreadMutexLock lock(state_.cs_);
    if (isOk() && isStarted() && !isDetached())
    {
        void *retval = NULL;
        if (::pthread_cancel(state_.id_) == 0)
        {
            ::pthread_join(state_.id_, &retval);
        }
    }
    ::pthread_attr_destroy(&state_.attr_);
    setStatus(NOTOK);
}

// set/get attributes
bool
SimpleThread::isDetached() const
{
    int state;
    ::pthread_attr_getdetachstate(&state_.attr_, &state);
    return(state == PTHREAD_CREATE_DETACHED);
}

bool
SimpleThread::isJoinable() const
{
    int state;
    ::pthread_attr_getdetachstate(&state_.attr_, &state);
    return(state == PTHREAD_CREATE_JOINABLE);
}

void
SimpleThread::setDetachedAttr(bool flag)
{
    if (isNotOk() || isStarted()) return;
    ThreadMutexLock lock(state_.cs_);
    if (flag)
        ::pthread_attr_setdetachstate(&state_.attr_, PTHREAD_CREATE_DETACHED);
    else
        ::pthread_attr_setdetachstate(&state_.attr_, PTHREAD_CREATE_JOINABLE);
}

// maintenance
int
SimpleThread::detach()
{
    if (isNotOk() || !isStarted()) return(-1);
    ThreadMutexLock lock(state_.cs_);
    ::pthread_detach(state_.id_);
    ::pthread_attr_setdetachstate(&state_.attr_, PTHREAD_CREATE_DETACHED);
    return(0);
}

int
SimpleThread::join(void *&retval)
{
    if (isNotOk() || !isStarted() || isDetached()) return(-1);
    retval = NULL;
    int status = ::pthread_join(state_.id_, &retval);
    ThreadMutexLock lock(state_.cs_);
    setOk(false);
    return(status);
}

int
SimpleThread::cancel()
{
    if (isNotOk() || !isStarted() || isDetached()) return(-1);
    int status = ::pthread_cancel(state_.id_);
    ThreadMutexLock lock(state_.cs_);
    setOk(status == 0);
    return(status);
}


int
SimpleThread::run()
{
    if (isNotOk() || isStarted()) return(-1);
    int status = ::pthread_create(&state_.id_, &state_.attr_, execute, this);
    ThreadMutexLock lock(state_.cs_);
    setOk(status == 0);
    return(status);
}

}
