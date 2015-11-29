// iterative server class

// headers
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <MultiProcessServer.h>

namespace ombt {

// ctors and dtor
MultiProcessDatagramServer::MultiProcessDatagramServer(
    EndPoint &serverep, 
    MultiProcessDatagramServer::Handler &handler):
        Server(), serverep_(serverep), handler_(handler)
{
    if (serverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

MultiProcessDatagramServer::~MultiProcessDatagramServer()
{
    ok_ = false;
}

// operations
int
MultiProcessDatagramServer::init()
{
    if ((serverep_.setReuseAddr(true) != 0) ||
        (serverep_.bind() != 0))
        return(-1);
    else
        return(0);
}

static void 
reaper(int sig)
{
    int status;
    while (::wait3(&status, WNOHANG, (struct rusage *)0) >= 0) ;
    (void) signal(SIGCHLD, reaper);
}

int
MultiProcessDatagramServer::run()
{
    if (!ok_) return(-1);

    char buf[BUFSIZ+1];
    size_t count;

    UseCntPtr<Address> peer_address(serverep_.getInternalAddress().create());

    (void) signal(SIGCHLD, reaper);

    while ( true )
    {
        count = BUFSIZ;
        count = serverep_.read(buf, count, *peer_address);
        if (count > 0)
        {
            int pid = fork();
            if (pid == 0)
            {
                // child
                int status = handler_(serverep_, *peer_address, buf, count);
                serverep_.close();
                exit(status);
            }
            else if (pid < 0)
            {
                // error in parent
                return(-1);
            }
        }
        else if (count == 0)
        {
             // end-of-file -- done.
             break;
        }
        else
        {
            // error. ignore interrupts
            if (errno == EINTR)
                continue;
            else
                return(-1);
        }
    }

    return(0);
}

int
MultiProcessDatagramServer::finish()
{
    return(serverep_.close());
}

// child process server ctor and dtor
MultiProcessStreamServer::SubServer::SubServer(
    EndPoint &subserverep, 
    MultiProcessStreamServer::Handler &handler):
        Server(), subserverep_(subserverep), handler_(handler)
{
    if (subserverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

MultiProcessStreamServer::SubServer::~SubServer()
{
    ok_ = false;
}

// operations
int
MultiProcessStreamServer::SubServer::init()
{
    return(0);
}

int
MultiProcessStreamServer::SubServer::run()
{
    if (!ok_) return(-1);
    int status;
    while ((status = handler_(subserverep_)) == 0) ;
    return(status < 0 ? -1 : 0);
}

int
MultiProcessStreamServer::SubServer::finish()
{
    return(subserverep_.close());
}

// ctor and dtor
MultiProcessStreamServer::MultiProcessStreamServer(
    EndPoint &serverep, 
    MultiProcessStreamServer::Handler &handler):
        Server(), serverep_(serverep), handler_(handler)
{
    if (serverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

MultiProcessStreamServer::~MultiProcessStreamServer()
{
    ok_ = false;
}

// operations
int
MultiProcessStreamServer::init()
{
    if ((serverep_.setReuseAddr(true) != 0) ||
        (serverep_.bind() != 0) || 
        (serverep_.listen() != 0))
        return(-1);
    else
        return(0);
}

int
MultiProcessStreamServer::run()
{
    if (!ok_) return(-1);

    UseCntPtr<EndPoint> peer;

    (void) signal(SIGCHLD, reaper);

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

        int pid = fork();
        if (pid == 0)
        {
            int status;

            // child -- close parent socket
            serverep_.close();

            // let handler do its work
            SubServer child(*peer, handler_);
            if ((status = child.init()) != 0) exit(status);
            if ((status = child.run()) != 0) exit(status);
            if ((status = child.finish()) != 0) exit(status);

            // all done
            exit(0);
        }
        else if (pid < 0)
        {
            // error in parent
            peer->close();
            return(-1);
        }
        else
        {
            // close child socket in parent
            peer->close();
        }
    }

    return(0);
}

int
MultiProcessStreamServer::finish()
{
    return(serverep_.close());
}

}
