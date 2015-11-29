// socket class

// headers
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <Debug.h>
#include <sockets/Socket.h>
#include <sockets/EndPoint.h>

namespace ombt {

// base socket ctor and dtor
Socket::Socket():
    BaseObject(false),
    socket_(-1), domain_(-1), 
    type_(-1), protocol_(-1)
{
    // do nothing
}

Socket::Socket(int socket):
    BaseObject(true),
    socket_(socket), domain_(-1), 
    type_(-1), protocol_(-1)
{
    // do nothing
}

Socket::Socket(int domain, int type, int protocol):
    BaseObject(false),
    socket_(-1), domain_(domain), 
    type_(type), protocol_(protocol)
{
    socket_ = ::socket(domain_, type_, protocol_);
    ok_ = (socket_ >= 0);
}

Socket::Socket(int socket, int domain, int type, int protocol):
    BaseObject(true),
    socket_(socket), domain_(domain), 
    type_(type), protocol_(protocol)
{
    ok_ = (socket_ >= 0);
}

Socket::Socket(const Socket &src):
    BaseObject(src),
    socket_(src.socket_), domain_(src.domain_),
    type_(src.type_), protocol_(src.protocol_)
{
    // do nothing
}

Socket::~Socket()
{
    // call close explicitly
    socket_ = -1;
    domain_ = -1;
    type_ = -1;
    protocol_ = -1;
    ok_ = false;
}

// assignment
Socket &
Socket::operator=(const Socket &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        socket_ = rhs.socket_;
        domain_ = rhs.domain_;
        type_ = rhs.type_;
        protocol_ = rhs.protocol_;
    }
    return(*this);
}

// basic socket operations
int
Socket::setReuseAddr(bool onoff)
{
    if (!ok_) return(-1);
    int flag = onoff ? 1 : 0;
    return(::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)));
}

int
Socket::setNonBlocking(bool onoff)
{
    if (!ok_) return(-1);
    int flag = ::fcntl(socket_, F_GETFL, 0);
    if (onoff)
        flag |= O_NONBLOCK;
    else
        flag &= ~O_NONBLOCK;
    return(::fcntl(socket_, F_SETFL, flag));
}

int
Socket::bind(const Address &address)
{
    if (address.isOk() && ok_ && 
        ::bind(socket_, address.getSockAddr(), 
               address.getSockAddrLen()) == 0)
    {
        ok_ = true;
        return(0);
    }
    else
    {
        ok_ = false;
        return(-1);
    }
}

int
Socket::listen(int backlog)
{
    return(-1);
}

EndPoint *
Socket::accept(bool &retry)
{
    retry = false;
    return(NULL);
}

int
Socket::connect(Address &address)
{
    if (ok_)
        return(::connect(socket_, 
                         address.getSockAddr(), 
                         address.getSockAddrLen()));
    else
        return(-1);
}

int
Socket::read(void *inbuffer, size_t &count)
{
    if (ok_)
        return(::read(socket_, inbuffer, count));
    else
        return(-1);
}

int
Socket::read(void *inbuffer, size_t &count, Address &peer_addr)
{
    if (ok_)
        return(::recvfrom(socket_, inbuffer, count, 0,
                          peer_addr.getSockAddr(), 
                          peer_addr.getSockAddrLenRef()));
    else
        return(-1);
}

int
Socket::write(const void *outbuffer, size_t count)
{
    if (ok_)
        return(::write(socket_, outbuffer, count));
    else
        return(-1);
}

int
Socket::write(const void *outbuffer, size_t count, const Address &peer_addr)
{
    if (ok_)
        return(::sendto(socket_, outbuffer, count, 0,
                        peer_addr.getSockAddr(), 
                        peer_addr.getSockAddrLen()));
    else
        return(-1);
}

int
Socket::readn(void *inbuffer, size_t &count)
{
    if (!ok_) return(-1);

    char *ptr = (char *)inbuffer;
    size_t nleft = count;

    size_t nread;
    while (nleft > 0)
    {
        if ((nread = ::read(socket_, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
                nread = 0;
            else
                return(-1);
        }
        else if (nread == 0)
        {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }

    return(count - nread);
}

int
Socket::writen(const void *outbuffer, size_t count)
{
    if (!ok_) return(-1);

    char *ptr = (char *)outbuffer;
    size_t nleft = count;

    size_t nwritten;
    while (nleft > 0)
    {
        if ((nwritten = ::write(socket_, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
                nwritten = 0;
            else
                return(-1);
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return(count);
}

int
Socket::open()
{
    if (socket_ != -1)
    {
        errno = EISCONN;
        return(-1);
    }
    socket_ = ::socket(domain_, type_, protocol_);
    if (socket >= 0)
    {
        ok_ = true;
        return(0);
    }
    else
    {
        socket_ = -1;
        ok_ = false;
        return(-1);
    }
}

int
Socket::close()
{
    if (ok_ && ::close(socket_) == 0)
    {
        socket_ = -1;
        ok_ = false;
        return(0);
    }
    else
    {
        ok_ = false;
        return(-1);
    }
}

// TCP socket ctor and dtor
TcpSocket::TcpSocket():
    Socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)
{
    // do nothing
}

TcpSocket::TcpSocket(int socket):
    Socket(socket, PF_INET, SOCK_STREAM, IPPROTO_TCP)
{
    // do nothing
}

TcpSocket::~TcpSocket()
{
    // do nothing
}

// assignment
TcpSocket &
TcpSocket::operator=(const TcpSocket &rhs)
{
    if (this != &rhs)
    {
        Socket::operator=(rhs);
    }
    return(*this);
}

// TCP-specific basic socket operations
int
TcpSocket::listen(int backlog)
{
    if (ok_ && backlog >= 0 && ::listen(socket_, backlog) == 0)
    {
        ok_ = true;
        return(0);
    }
    else
    {
        return(-1);
    }
}

EndPoint *
TcpSocket::accept(bool &retry)
{
    retry = false;
    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = ::accept(socket_, (sockaddr *)&client_address,
                                 &client_address_len);
    if (client_socket < 0)
    {
        // during heavy traffic, the client may timeout
        // abort
        if (errno == EWOULDBLOCK || 
            errno == EINTR ||
            errno == ECONNABORTED) retry = true;
        return(NULL);
    }

    return(new EndPoint(new TcpSocket(client_socket), 
                        new InetAddress(client_address)));
}

// UDP socket ctor and dtor
UdpSocket::UdpSocket():
    Socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)
{
    // do nothing
}

UdpSocket::UdpSocket(int socket):
    Socket(socket, PF_INET, SOCK_DGRAM, IPPROTO_UDP)
{
    // do nothing
}

UdpSocket::~UdpSocket()
{
    // do nothing
}

// assignment
UdpSocket &
UdpSocket::operator=(const UdpSocket &rhs)
{
    if (this != &rhs)
    {
        Socket::operator=(rhs);
    }
    return(*this);
}

// Local/Unix stream ctor and dtor
StreamSocket::StreamSocket():
    Socket(PF_LOCAL, SOCK_STREAM, 0)
{
    // do nothing
}

StreamSocket::StreamSocket(int socket):
    Socket(socket, PF_LOCAL, SOCK_STREAM, 0)
{
    // do nothing
}

StreamSocket::~StreamSocket()
{
    // do nothing
}

// assignment
StreamSocket &
StreamSocket::operator=(const StreamSocket &rhs)
{
    if (this != &rhs)
    {
        Socket::operator=(rhs);
    }
    return(*this);
}

// unix-specific basic socket operations
int
StreamSocket::listen(int backlog)
{
    if (ok_ && backlog >= 0 && ::listen(socket_, backlog) == 0)
    {
        ok_ = true;
        return(0);
    }
    else
    {
        return(-1);
    }
}

EndPoint *
StreamSocket::accept(bool &retry)
{
    retry = false;
    sockaddr_un client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = ::accept(socket_, (sockaddr *)&client_address,
                                 &client_address_len);
    if (client_socket < 0)
    {
        // during heavy traffic, the client may timeout
        // abort
        if (errno == EWOULDBLOCK ||
            errno == ECONNABORTED) retry = true;
        return(NULL);
    }

    return(new EndPoint(new StreamSocket(client_socket), 
                        new LocalAddress(client_address)));
}

// Local/Unix datagram ctor and dtor
DatagramSocket::DatagramSocket():
    Socket(PF_LOCAL, SOCK_DGRAM, 0)
{
    // do nothing
}

DatagramSocket::DatagramSocket(int socket):
    Socket(socket, PF_LOCAL, SOCK_DGRAM, 0)
{
    // do nothing
}

DatagramSocket::~DatagramSocket()
{
    // do nothing
}

// assignment
DatagramSocket &
DatagramSocket::operator=(const DatagramSocket &rhs)
{
    if (this != &rhs)
    {
        Socket::operator=(rhs);
    }
    return(*this);
}


}
