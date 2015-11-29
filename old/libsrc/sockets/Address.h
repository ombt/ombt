#ifndef __OMBT_ADDRESS_H
#define __OMBT_ADDRESS_H

// headers
#include <string>
#include <sys/socket.h>
#include <atomic/BaseObject.h>

namespace ombt {

// address class
class Address: public BaseObject {
public:
    Address(const std::string &address = "");
    Address(const Address &src);
    virtual ~Address();

    virtual Address *create() const;

    Address &operator=(const Address &rhs);

    inline virtual sockaddr *getSockAddr() const {
        return(NULL);
    };
    inline virtual socklen_t getSockAddrLen() const {
        return(sock_addr_len_);
    };
    inline virtual socklen_t *getSockAddrLenRef() const {
        return(&sock_addr_len_);
    };
    inline virtual const std::string &getAddress() const {
        return(address_);
    };
    inline virtual void setAddress(const std::string &address) {
        address_ = address;
    }

protected:
    mutable socklen_t sock_addr_len_;
    std::string address_;
};

}

#endif
