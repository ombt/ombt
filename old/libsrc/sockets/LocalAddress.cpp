// local/unix address class

// headers
#include <string.h>
#include <sockets/LocalAddress.h>

namespace ombt {

// ctor and dtor
LocalAddress::LocalAddress():
    Address()
{
    ::memset(&local_address_, 0, sizeof(local_address_));
    ok_ = true;
}

LocalAddress::LocalAddress(const std::string &address):
    Address(address)
{
    ok_ = false;
    setLocalAddress();
}

LocalAddress::LocalAddress(const LocalAddress &src):
    Address(src)
{
    memcpy(&local_address_, &src.local_address_, sizeof(local_address_));
}

LocalAddress::LocalAddress(const sockaddr_un &local_address):
    Address()
{
    ok_ = true;
    memcpy(&local_address_, &local_address, sizeof(local_address));
}

LocalAddress::~LocalAddress()
{
    // do nothing
}

LocalAddress *
LocalAddress::create() const
{
    return(new LocalAddress());
}

// assignment operator
LocalAddress &
LocalAddress::operator=(const LocalAddress &rhs)
{
    if (this != &rhs)
    {
        Address::operator=(rhs);
        memcpy(&local_address_, &rhs.local_address_, sizeof(local_address_));
    }
    return(*this);
}

void
LocalAddress::setLocalAddress()
{
    ::memset(&local_address_, 0, sizeof(local_address_));

    local_address_.sun_family = AF_LOCAL;

    if (address_ != "")
    {
        ok_ = true;
        memcpy(local_address_.sun_path, address_.c_str(), address_.length());
    }
    else
    {
        ok_ = false;
    }
}

}

