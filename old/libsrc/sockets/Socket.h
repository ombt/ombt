#ifndef __OMBT_SOCKET_H
#define __OMBT_SOCKET_H

// headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <atomic/BaseObject.h>
#include <sockets/Address.h>
#include <sockets/InetAddress.h>
#include <sockets/LocalAddress.h>

namespace ombt {

// forward declaration
class EndPoint;

// socket class
class Socket: public BaseObject {
public:
    enum {
         DefaultQueueSize = 5,
    };

    Socket();
    Socket(int socket);
    Socket(int domain, int type, int protocol);
    Socket(int socket, int domain, int type, int protocol);
    Socket(const Socket &src);
    virtual ~Socket();

    // assignment and comparison
    Socket &operator=(const Socket &rhs);
    bool operator<(const Socket &rhs) {
       return(socket_ < rhs.socket_);
    }

    // accessor
    int getSocket() const { return(socket_); }

    // basic socket operations
    virtual int setReuseAddr(bool onoff);
    virtual int setNonBlocking(bool onoff);
    virtual int bind(const Address &address);
    virtual int listen(int backlog = DefaultQueueSize);
    virtual EndPoint *accept(bool &retry);
    virtual int connect(Address &address);
    virtual int read(void *inbuffer, size_t &count);
    virtual int read(void *inbuffer, size_t &count, Address &peer_addr);
    virtual int write(const void *outbuffer, size_t count);
    virtual int write(const void *outbuffer, size_t count, const Address &peer_addr);
    virtual int readn(void *inbuffer, size_t &count);
    virtual int writen(const void *outbuffer, size_t count);
    virtual int open();
    virtual int close();

    // set/clear FS sets
    void setFDSet(fd_set &fdset) const {
        FD_SET(socket_, &fdset);
    }
    void clearFDSet(fd_set &fdset) const {
        FD_CLR(socket_, &fdset);
    }

protected:
    inline bool isSocketSane() const {
        if (!ok_)
        {
            return(false);
        }
        else if (socket_ < 0)
        {
            ok_ = false;
            return(false);
        }
        return(true);
    }

protected:
    int socket_;
    int domain_;
    int type_;
    int protocol_;
};

// TCP IPv4 socket class
class TcpSocket: public Socket {
public:
    TcpSocket();
    TcpSocket(int socket);
    TcpSocket(const TcpSocket &src);
    ~TcpSocket();

    // basic socket operations
    int listen(int backlog);
    EndPoint *accept(bool &retry);

    // assignment
    TcpSocket &operator=(const TcpSocket &rhs);
};

// UDP IPv4 socket class
class UdpSocket: public Socket {
public:
    UdpSocket();
    UdpSocket(int socket);
    UdpSocket(const UdpSocket &src);
    ~UdpSocket();

    // assignment
    UdpSocket &operator=(const UdpSocket &rhs);
};

// Local/Unix stream socket class
class StreamSocket: public Socket {
public:
    StreamSocket();
    StreamSocket(int socket);
    StreamSocket(const StreamSocket &src);
    ~StreamSocket();

    // basic socket operations
    int listen(int backlog);
    EndPoint *accept(bool &retry);

    // assignment
    StreamSocket &operator=(const StreamSocket &rhs);
};

// Local/Unix datagram socket class
class DatagramSocket: public Socket {
public:
    DatagramSocket();
    DatagramSocket(int socket);
    DatagramSocket(const DatagramSocket &src);
    ~DatagramSocket();

    // assignment
    DatagramSocket &operator=(const DatagramSocket &rhs);
};

}

#endif
