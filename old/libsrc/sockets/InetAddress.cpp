// IPv4 internet address class

// headers
#include <sockets/InetAddress.h>

namespace ombt {

// ctor and dtor
InetAddress::InetAddress():
    Address(), port_(-1)
{
    ::memset(&inet_address_, 0, sizeof(inet_address_));
    ok_ = true;
}

InetAddress::InetAddress(int port):
    Address(), port_(port)
{
    ok_ = false;
    setInetAddress();

}

InetAddress::InetAddress(const std::string &address, int port):
    Address(address), port_(port)
{
    ok_ = false;
    setInetAddress();
}

InetAddress::InetAddress(const InetAddress &src):
    Address(src), port_(src.port_)
{
    memcpy(&inet_address_, &src.inet_address_, sizeof(inet_address_));
}

InetAddress::InetAddress(const sockaddr_in &inet_address):
    Address(), port_(-1)
{
    ok_ = true;
    port_ = ntohs(inet_address.sin_port);
    memcpy(&inet_address_, &inet_address, sizeof(inet_address));
}

InetAddress::~InetAddress()
{
    // do nothing
}

InetAddress *
InetAddress::create() const
{
    return(new InetAddress());
}


// assignment operator
InetAddress &
InetAddress::operator=(const InetAddress &rhs)
{
    if (this != &rhs)
    {
        Address::operator=(rhs);
        port_ = rhs.port_;
        memcpy(&inet_address_, &rhs.inet_address_, sizeof(inet_address_));
    }
    return(*this);
}

void
InetAddress::setInetAddress()
{
    ::memset(&inet_address_, 0, sizeof(inet_address_));

    inet_address_.sin_family = AF_INET;
    inet_address_.sin_port = htons(port_);

    if (address_ != "")
    {
        if (inet_aton(address_.c_str(), &inet_address_.sin_addr) == 0)
        {
            hostent *hp = gethostbyname(address_.c_str());
            if (hp != NULL)
            {
                inet_address_.sin_addr = *(in_addr *)hp->h_addr;
                ok_ = true;
            }
        }
        else
        {
            ok_ = true;
        }
    }
    else
    {
        inet_address_.sin_addr.s_addr = htonl(INADDR_ANY);
        ok_ = true;
    }
}

}

