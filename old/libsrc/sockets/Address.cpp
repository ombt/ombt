// address class

// headers
#include <sockets/Address.h>

namespace ombt {

// ctor and dtor
Address::Address(const std::string &address):
    BaseObject(), 
    address_(address), sock_addr_len_(0)
{
    ok_ = true;
}

Address::Address(const Address &src):
    BaseObject(src), 
    address_(src.address_), sock_addr_len_(src.sock_addr_len_)
{
    // do nothing
}

Address::~Address()
{
    // do nothing
}

Address *
Address::create() const
{
    return(new Address());
}

// assignment operator
Address &
Address::operator=(const Address &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        address_ = rhs.address_;
        sock_addr_len_ = rhs.sock_addr_len_;
    }

    return(*this);
}

}

