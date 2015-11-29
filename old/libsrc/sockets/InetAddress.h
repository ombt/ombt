#ifndef __OMBT_INET_ADDRESS_H
#define __OMBT_INET_ADDRESS_H

// headers
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sockets/Address.h>

namespace ombt {

// IPv4 internet address class
class InetAddress: public Address {
public:
    InetAddress();
    InetAddress(int port);
    InetAddress(const std::string &address, int port);
    InetAddress(const InetAddress &src);
    InetAddress(const sockaddr_in &address);
    ~InetAddress();

    InetAddress *create() const;

    InetAddress &operator=(const InetAddress &rhs);

    inline virtual int getPort() const {
        return(port_);
    };
    inline virtual void setPort(int port) {
        port_ = port;
    }

    inline sockaddr *getSockAddr() const {
        return((sockaddr *)&inet_address_);
    }
    inline socklen_t getSockAddrLen() const {
        sock_addr_len_ = sizeof(sockaddr_in);
        return(sock_addr_len_);
    }
    inline socklen_t *getSockAddrLenRef() const {
        sock_addr_len_ = sizeof(sockaddr_in);
        return(&sock_addr_len_);
    }
    inline const sockaddr_in *getInetAddress() const {
        return(&inet_address_);
    }

protected:
    void setInetAddress();

protected:
    int port_;
    sockaddr_in inet_address_;
};

}

#endif

