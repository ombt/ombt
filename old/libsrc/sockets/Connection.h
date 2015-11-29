#ifndef __OMBT_CONNECTION_H
#define __OMBT_CONNECTION_H

// system headers
#include <string>

// local headers
#include <atomic/UseCntPtr.h>
#include <atomic/BaseObject.h>
#include <sockets/Address.h>
#include <sockets/Socket.h>

namespace ombt {

// connection class - socket and address combined.
class Connection: public BaseObject {
public:
    Connection(Socket *sock, Address *addr);
    Connection(const Connection &src);
    ~Connection();

    // assignment
    Connection &operator=(const Connection &rhs);

    // for maps
    bool operator<(const Connection &rhs) const {
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

    // socket-base operations 
    int bind();
    int listen(int backlog = Socket::DefaultQueueSize);
    Connection *accept();
    int connect(Address &address);
    int read(void *inbuffer, size_t &count);
    int read(void *inbuffer, size_t &count, Address &peer_addr);
    int write(void *outbuffer, size_t count);
    int write(void *outbuffer, size_t count, const Address &peer_addr);
    int readn(void *inbuffer, size_t &count);
    int writen(void *outbuffer, size_t count);
    int close();

private:
    // not allowed
    Connection();

protected:
    UseCntPtr<Socket> socket_;
    UseCntPtr<Address> address_;
};

}

#endif 

