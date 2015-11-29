#ifndef __OMBT_SERVICE_H
#define __OMBT_SERVICE_H

// headers
#include <stdlib.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <atomic/BaseObject.h>
#include <parsing/Tokenizer.h>

namespace ombt {

// services class
class Service: public BaseObject {
public:
    Service();
    Service(const std::string &uri);
    Service(const Service &src);
    ~Service();

    // assignment and comparison
    Service &operator=(const Service &rhs);
    bool operator<(const Service &rhs) {
       return(uri_ < rhs.uri_);
    }

    // accessor
    void setUri(const std::string &uri) { uri_ = uri; parser(); }
    const std::string getUri() const { return(uri_); }
    const std::string getHost() const { return(host_); }
    const std::string getService() const { return(service_); }
    const std::string getProtocol() const { return(protocol_); }
    const int getPort() const { return(port_); }

private:
    void parser();

    std::string uri_;
    std::string host_;
    std::string service_;
    std::string protocol_;
    int port_;
};

}

#endif
