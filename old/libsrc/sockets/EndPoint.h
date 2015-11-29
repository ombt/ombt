#ifndef __OMBT_CONNECTION_H
#define __OMBT_CONNECTION_H

// system headers
#include <string>
#include <iostream>

// local headers
#include <atomic/BaseObject.h>
#include <atomic/UseCntPtr.h>
#include <sockets/Address.h>
#include <sockets/Socket.h>

namespace ombt {

// connection class - socket and address combined.
class EndPoint: public BaseObject {
public:
    EndPoint(Socket *sock = NULL, Address *addr = NULL);
    EndPoint(const EndPoint &src);
    ~EndPoint();

    // assignment
    EndPoint &operator=(const EndPoint &rhs);

    // for maps
    bool operator<(const EndPoint &rhs) const {
        return(socket_ < rhs.socket_);
    }

    // internal status
    bool isOk();

    // accessor methods
    inline const sockaddr *getSockAddr() const {
        return(address_->getSockAddr());
    };
    inline socklen_t getSockAddrLen() const {
        return(address_->getSockAddrLen());
    };
    inline const std::string &getAddress() const {
        return(address_->getAddress());
    };

    int getSocket() const;
    Address &getInternalAddress();

    // socket-base operations 
    int setReuseAddr(bool onoff) {
        return(socket_->setReuseAddr(onoff));
    }
    int setNonBlocking(bool onoff) {
        return(socket_->setNonBlocking(onoff));
    }
    int bind();
    int listen(int backlog = Socket::DefaultQueueSize);
    EndPoint *accept(bool &retry);
    int connect(Address &address);
    int read(void *inbuffer, size_t &count);
    int read(void *inbuffer, size_t &count, Address &peer_addr);
    int write(void *outbuffer, size_t count);
    int write(void *outbuffer, size_t count, const Address &peer_addr);
    int readn(void *inbuffer, size_t &count);
    int writen(void *outbuffer, size_t count);
    int close();
    int open();

    friend std::ostream &operator<<(std::ostream &os, EndPoint &) {
        return(os);
    }

private:
    // not allowed
    // EndPoint();

protected:
    UseCntPtr<Socket> socket_;
    UseCntPtr<Address> address_;
};

}

#endif 

