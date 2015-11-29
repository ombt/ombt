// iterative server class

// headers
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <MultiThreadServer.h>

namespace ombt {

// thread ctor and dtor
        // ctor and dtor
MultiThreadStreamServer::Task::Task(EndPoint &subserverep, Handler &handler):
    SimpleThread::Task(), subserverep_(subserverep_), handler_(handler)
{
    if (subserverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

MultiThreadStreamServer::Task::~Task() { BaseObject::ok_ = false; }

int
MultiThreadStreamServer::Task::init()
{
    if (ok_)
        return(0);
    else
        return(-1);
}

int
MultiThreadStreamServer::Task::run()
{
    if (!ok_) return(-1);
    int status;
    while ((status = handler_(subserverep_)) == 0) ;
    return(status < 0 ? -1 : 0);
}

int MultiThreadStreamServer::Task::finish()
{
    return(subserverep_.close());
}

// ctor and dtor
MultiThreadStreamServer::MultiThreadStreamServer(
    EndPoint &serverep, 
    MultiThreadStreamServer::Handler &handler):
        Server(), serverep_(serverep), handler_(handler)
{
    if (serverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

MultiThreadStreamServer::~MultiThreadStreamServer()
{
    ok_ = false;
}

// operations
int
MultiThreadStreamServer::init()
{
    if ((serverep_.setReuseAddr(true) != 0) ||
        (serverep_.bind() != 0) || 
        (serverep_.listen() != 0))
        return(-1);
    else
        return(0);
}

int
MultiThreadStreamServer::run()
{
    if (!ok_) return(-1);

    UseCntPtr<EndPoint> peer;

    while ( 1 )
    {
        bool retry = false;
        peer = serverep_.accept(retry);
        if (peer.ptr() == NULL)
        {
            if (retry)
                continue; // ignore interrupt from child HANGUP
            else
                return(-1);
        }

        Task task(*peer, handler_);

        SimpleThread thread(new Task(*peer, handler_));
        MustBeTrue(thread.isOk());

        thread.setDetachedAttr(true);
        MustBeTrue(thread.run() == 0);
    }

    return(0);
}

int
MultiThreadStreamServer::finish()
{
    return(serverep_.close());
}

}
