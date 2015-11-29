// connection class - socket and address combined

// headers
#include <Debug.h>
#include <sockets/Socket.h>
#include <sockets/Connection.h>

namespace ombt {

// ctors and dtor
Connection::Connection(Socket *sock, Address *addr):
    BaseObject(), 
    socket_(sock), address_(addr)
{
    if (!socket_.isNull() && !address_.isNull())
        ok_= (socket_->isOk() && address_->isOk());
    else if (!socket_.isNull() && address_.isNull())
        ok_= socket_->isOk();
    else
        ok_ = false;
}

Connection::Connection(const Connection &src):
    BaseObject(src), 
    socket_(src.socket_), address_(src.address_)
{
    // do nothing
}

Connection::~Connection()
{
    ok_ = false;
}

// assignment
Connection &
Connection::operator=(const Connection &rhs)
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
Connection::isOk()
{
    if (!socket_.isNull() && !address_.isNull())
        return(ok_= (socket_->isOk() && address_->isOk()));
    else if (!socket_.isNull() && address_.isNull())
        return(ok_= socket_->isOk());
    else
        return(ok_ = false);
}

int 
Connection::getSocket() const
{
    if (ok_ && !socket_.isNull())
        return(socket_->getSocket());
    else
        return(-1);
}

// socket operations
int
Connection::bind()
{
    if (ok_ && !socket_.isNull() && !address_.isNull())
        return(socket_->bind(*address_));
    else
        return(-1);
}

int
Connection::listen(int backlog)
{
    if (ok_ && !socket_.isNull() && !address_.isNull())
        return(socket_->listen(backlog));
    else
        return(-1);
}

Connection *
Connection::accept()
{
    if (ok_ && !socket_.isNull() && !address_.isNull())
        return(socket_->accept());
    else
        return(NULL);
}

int
Connection::connect(Address &address)
{
    if (ok_ && !socket_.isNull())
        return(socket_->connect(address));
    else
        return(-1);
}

int
Connection::read(void *inbuffer, size_t &count)
{
    if (ok_ && !socket_.isNull())
        return(socket_->read(inbuffer, count));
    else
        return(-1);
}

int
Connection::readn(void *inbuffer, size_t &count)
{
    if (ok_ && !socket_.isNull())
        return(socket_->readn(inbuffer, count));
    else
        return(-1);
}

int
Connection::read(void *inbuffer, size_t &count, Address &peer_address)
{
    if (ok_ && !socket_.isNull())
        return(socket_->read(inbuffer, count, peer_address));
    else
        return(-1);
}

int
Connection::write(void *outbuffer, size_t count)
{
    if (ok_ && !socket_.isNull())
        return(socket_->write(outbuffer, count));
    else
        return(-1);
}

int
Connection::writen(void *outbuffer, size_t count)
{
    if (ok_ && !socket_.isNull())
        return(socket_->writen(outbuffer, count));
    else
        return(-1);
}

int
Connection::write(void *outbuffer, size_t count, const Address &peer_address)
{
    if (ok_ && !socket_.isNull())
        return(socket_->write(outbuffer, count, peer_address));
    else
        return(-1);
}

int
Connection::close()
{
    if (ok_ && !socket_.isNull())
        return(socket_->close());
    else
        return(-1);
}

}
