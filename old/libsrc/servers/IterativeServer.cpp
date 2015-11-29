// iterative server class

// headers
#include <errno.h>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <IterativeServer.h>

namespace ombt {

// ctors and dtor
IterativeDatagramServer::IterativeDatagramServer(
    EndPoint &serverep, 
    IterativeDatagramServer::Handler &handler):
        Server(), serverep_(serverep), handler_(handler)
{
    if (serverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

IterativeDatagramServer::~IterativeDatagramServer()
{
    ok_ = false;
}

// operations
int
IterativeDatagramServer::init()
{
    if ((serverep_.setReuseAddr(true) != 0) ||
        (serverep_.bind() != 0))
    {
        return(-1);
    }
    else
    {
        return(0);
    }
}

int
IterativeDatagramServer::run()
{
    if (!ok_) return(-1);

    char buf[BUFSIZ+1];
    size_t count = BUFSIZ;

    UseCntPtr<Address> peer_address(serverep_.getInternalAddress().create());

    while (serverep_.read(buf, count, *peer_address) > 0)
    {
        int status = handler_(serverep_, *peer_address, buf, count);
        if (status < 0)
        {
            // error of some type
            return(-1);
        }
        else if (status > 0)
        {
            // done, just exit.
            break;
        }
    }

    return(0);
}

int
IterativeDatagramServer::finish()
{
    return(serverep_.close());
}

// ctor and dtor
IterativeStreamServer::IterativeStreamServer(
    EndPoint &serverep, 
    IterativeStreamServer::Handler &handler):
        Server(), serverep_(serverep), handler_(handler)
{
    if (serverep_.isOk() && handler_.isOk())
        ok_ = true;
    else
        ok_ = false;
}

IterativeStreamServer::~IterativeStreamServer()
{
    ok_ = false;
}

// operations
int
IterativeStreamServer::init()
{
    if ((serverep_.setReuseAddr(true) != 0) ||
        (serverep_.bind() != 0) || 
        (serverep_.listen() != 0))
    {
        return(-1);
    }
    else
    {
        return(0);
    }
}

int
IterativeStreamServer::run()
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
            {
                continue;
            }
            else
            {
                return(-1);
            }
        }

        (void) handler_(*peer);

        peer->close();
    }

    return(0);
}

int
IterativeStreamServer::finish()
{
    return(serverep_.close());
}

}
