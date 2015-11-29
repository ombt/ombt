//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// iterative server class

// system headers
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

// local headers
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <MultiThreadServer.h>

namespace ombt {

// thread ctor and dtor
        // ctor and dtor
MultiThreadStreamServer::Task::Task(EndPoint *psubserverep, Handler *phandler):
    SimpleThread::Task(), psubserverep_(psubserverep), phandler_(phandler)
{
    if (psubserverep_->isOk() && phandler_->isOk())
        setOk(true);
    else
        setOk(false);
}

MultiThreadStreamServer::Task::~Task() { setOk(false); }

int
MultiThreadStreamServer::Task::init()
{
    if (isOk())
        return(OK);
    else
        return(NOTOK);
}

int
MultiThreadStreamServer::Task::run()
{
    if (isNotOk()) return(NOTOK);
    int status;
    while ((status = (*phandler_)(psubserverep_)) == OK) ;
    return(status < 0 ? NOTOK : OK);
}

int MultiThreadStreamServer::Task::finish()
{
    return(psubserverep_->close());
}

// ctor and dtor
MultiThreadStreamServer::MultiThreadStreamServer(
    EndPoint *pserverep, 
    MultiThreadStreamServer::Handler *phandler):
        Server(), pserverep_(pserverep), phandler_(phandler)
{
    if (pserverep_->isOk() && phandler_->isOk())
        setOk(true);
    else
        setOk(false);
}

MultiThreadStreamServer::~MultiThreadStreamServer()
{
    setOk(false);
}

// operations
int
MultiThreadStreamServer::init()
{
    if ((pserverep_->setReuseAddr(true) != 0) ||
        (pserverep_->bind() != 0) || 
        (pserverep_->listen() != 0))
        return(NOTOK);
    else
        return(OK);
}

int
MultiThreadStreamServer::run()
{
    if (isNotOk()) return(NOTOK);

    UseCntPtr<EndPoint> peer;

    while ( 1 )
    {
        bool retry = false;
        peer = pserverep_->accept(retry);
        if (peer == NULL)
        {
            if (retry)
                continue; // ignore interrupt from child HANGUP
            else
                return(NOTOK);
        }

        Task task(peer, phandler_);

        SimpleThread thread(new Task(peer, phandler_));
        MustBeTrue(thread.isOk());

        thread.setDetachedAttr(true);
        MustBeTrue(thread.run() == OK);
    }

    return(OK);
}

int
MultiThreadStreamServer::finish()
{
    return(pserverep_->close());
}

}
