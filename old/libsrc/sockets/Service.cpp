// iterative server class

// headers
#include <Service.h>

namespace ombt {

// ctors and dtor
Service::Service():
    BaseObject(), uri_(), host_(), service_(), protocol_(), port_(-1)
{
    ok_ = false;
}

Service::Service(const std::string &uri):
    BaseObject(), uri_(uri), host_(), service_(), protocol_(), port_(-1)
{
    parser();
}

Service::Service(const Service &src):
    BaseObject(src), 
    uri_(src.uri_), host_(src.host_), 
    service_(src.service_), protocol_(src.protocol_), 
    port_(src.port_)
{
}

Service::~Service()
{
    ok_ = false;
}

// assignment
Service &
Service::operator=(const Service &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        uri_ = rhs.uri_;
        host_ = rhs.host_;
        service_ = rhs.service_;
        protocol_ = rhs.protocol_;
        port_ = rhs.port_;
    }
    return(*this);
}

// parse uri
void
Service::parser()
{
    // initialize
    ok_ = false;
    host_ = "";
    service_ = "";
    protocol_ = "";
    port_ = -1;
    
    // split into parts
    //
    // address:service:protocol or 
    // address:port:protocol or
    // address:filepath:protocol
    //
    std::vector<std::string> tokens;
    Tokenizer tokenizer;
    if (tokenizer(uri_, tokens, ":") != 0) return;
    if (tokens.size() != 3) return;

    // assign for now
    host_ = tokens[0];
    service_ = tokens[1];
    protocol_ = tokens[2];

    // look at parts
    if (host_ == "") host_ = "localhost";

    if (protocol_ == "udp" || protocol_ == "tcp")
    {
        servent *pse;
        pse = ::getservbyname(service_.c_str(), protocol_.c_str());
        if (pse)
            port_ = ntohs(pse->s_port);
        else
        {
            port_ = (unsigned short)(atoi(service_.c_str()));
            if (port_ == 0) return;
        }
    }
    else if (protocol_ == "datagram" || protocol_ == "stream")
    {
        host_ = "localhost";
        port_ = -1;
    }
    else
        return;

    ok_ = true;
    return;
}

}
