// connection class - socket and address combined

// headers
#include <Debug.h>
#include <logging/Logging.h>
#include <sockets/Socket.h>
#include <sockets/EndPoint.h>

namespace ombt {

// ctors and dtor
EndPoint::EndPoint(Socket *sock, Address *addr):
    BaseObject(), 
    socket_(sock), address_(addr)
{
DBGTRACE();
    if (socket_.ptr() != NULL && address_.ptr() != NULL)
        ok_= (socket_->isOk() && address_->isOk());
    else if (socket_.ptr() != NULL && address_.ptr() == NULL)
        ok_= socket_->isOk();
    else
        ok_ = false;
}

EndPoint::EndPoint(const EndPoint &src):
    BaseObject(src), 
    socket_(src.socket_), address_(src.address_)
{
DBGTRACE();
    // do nothing
}

EndPoint::~EndPoint()
{
DBGTRACE();
    ok_ = false;
}

// assignment
EndPoint &
EndPoint::operator=(const EndPoint &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        socket_ = rhs.socket_;
        address_ = rhs.address_;
    }
    return(*this);
}

// accessor function
bool
EndPoint::isOk()
{
    if (socket_.ptr() != NULL && address_.ptr() != NULL)
        return(ok_= (socket_->isOk() && address_->isOk()));
    else if (socket_.ptr() != NULL && address_.ptr() == NULL)
        return(ok_= socket_->isOk());
    else
        return(ok_ = false);
}

int 
EndPoint::getSocket() const
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->getSocket());
    else
        return(-1);
}

Address &
EndPoint::getInternalAddress()
{
    return(*address_);
}


// socket operations
int
EndPoint::bind()
{
    if (ok_ && socket_.ptr() != NULL && address_.ptr() != NULL)
        return(socket_->bind(*address_));
    else
        return(-1);
}

int
EndPoint::listen(int backlog)
{
    if (ok_ && socket_.ptr() != NULL && address_.ptr() != NULL)
        return(socket_->listen(backlog));
    else
        return(-1);
}

EndPoint *
EndPoint::accept(bool &retry)
{
    retry = false;
    if (ok_ && socket_.ptr() != NULL && address_.ptr() != NULL)
        return(socket_->accept(retry));
    else
        return(NULL);
}

int
EndPoint::connect(Address &address)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->connect(address));
    else
        return(-1);
}

int
EndPoint::read(void *inbuffer, size_t &count)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->read(inbuffer, count));
    else
        return(-1);
}

int
EndPoint::readn(void *inbuffer, size_t &count)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->readn(inbuffer, count));
    else
        return(-1);
}

int
EndPoint::read(void *inbuffer, size_t &count, Address &peer_address)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->read(inbuffer, count, peer_address));
    else
        return(-1);
}

int
EndPoint::write(void *outbuffer, size_t count)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->write(outbuffer, count));
    else
        return(-1);
}

int
EndPoint::writen(void *outbuffer, size_t count)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->writen(outbuffer, count));
    else
        return(-1);
}

int
EndPoint::write(void *outbuffer, size_t count, const Address &peer_address)
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->write(outbuffer, count, peer_address));
    else
        return(-1);
}

int
EndPoint::close()
{
DBGTRACE();
    if (ok_ && socket_.ptr() != NULL)
    {
DBGPRINTF("EndPoint::close() socket=%d", socket_->getSocket());
        return(socket_->close());
    }
    else
        return(-1);
}

int
EndPoint::open()
{
    if (ok_ && socket_.ptr() != NULL)
        return(socket_->open());
    else
        return(-1);
}

}
