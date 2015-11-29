#ifndef __OMBT_MULTI_PROCESS_SERVER_PER_PROCESS_H
#define __OMBT_MULTI_PROCESS_SERVER_PER_PROCESS_H

// iterative single-service per process server

// headers
#include <stdio.h>
#include <set>
#include <map>
#include <list>
#include <string>
#include <servers/Server.h>
#include <sockets/Address.h>
#include <sockets/EndPoint.h>
#include <sockets/Service.h>
#include <servers/MultiProcessMultiServiceServer.h>

namespace ombt {

// iterative server for a connection
class MultiProcMSDatagramServerPerProcess: public Server
{
public:
    // ctors and dtor
    MultiProcMSDatagramServerPerProcess(int procnum=1, int maxstartups=2, Server::MultiplexMode mpmode = Server::Select);
    virtual ~MultiProcMSDatagramServerPerProcess();

    // register service and handlers
    // form of service is address:service:protocol
    // address: X.X.X.X, localhost, N.N.N.N
    // service: port or service name
    // protocol: udp or datagram
    //
    void registerService(const std::string &service, MultiProcMSDatagramServer::Handler &handler);

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    MultiProcMSDatagramServerPerProcess(const MultiProcMSDatagramServerPerProcess &src);
    MultiProcMSDatagramServerPerProcess &operator=(const MultiProcMSDatagramServerPerProcess &rhs);

protected:
    // track services
    typedef std::list<std::pair<std::string,
                      MultiProcMSDatagramServer::Handler * > > SVCSList;
    typedef SVCSList::const_iterator SVCSListConstIter;
    typedef SVCSList::iterator SVCSListIter;
    typedef std::map<const std::string, MultiProcMSDatagramServer::Handler * > SVCS;
    typedef SVCS::const_iterator SVCSConstIter;
    typedef SVCS::iterator SVCSIter;
    SVCS services_;

    // track service pids
    struct PID2ServiceData {
        int startups_;
        int maxstartups_;
        std::string uri_;
        std::string service_;
        std::string host_;
        std::string protocol_;
        int port_;
        MultiProcMSDatagramServer::Handler *handler_;

        PID2ServiceData(): 
            startups_(0), maxstartups_(-1), uri_(),
            service_(), host_(), protocol_(), port_(-1),
            handler_(NULL) { }
        PID2ServiceData(const PID2ServiceData &pd): 
            startups_(pd.startups_), maxstartups_(pd.maxstartups_), 
            uri_(pd.uri_), service_(pd.service_), host_(pd.host_),
            protocol_(pd.protocol_), port_(pd.port_), handler_(pd.handler_) { }
        PID2ServiceData(int startups, int maxstartups, const std::string &uri, 
                const std::string &service, const std::string &host, 
                const std::string &protocol, int port,
                MultiProcMSDatagramServer::Handler *handler):
                startups_(startups), maxstartups_(maxstartups), 
                uri_(uri), service_(service), host_(host),
                protocol_(protocol), port_(port), handler_(handler) { }
        PID2ServiceData &operator=(const PID2ServiceData &rhs) {
            if (this != &rhs)
            {
                startups_ = rhs.startups_;
                maxstartups_ = rhs.maxstartups_;
                uri_ = rhs.uri_;
                service_ = rhs.service_;
                host_ = rhs.host_;
                protocol_ = rhs.protocol_;
                port_ = rhs.port_;
                handler_ = rhs.handler_;
            }
            return(*this);
        }
    };

    typedef std::list<PID2ServiceData> SVCDATAList;
    typedef SVCDATAList::const_iterator SVCDATAListConstIter;
    typedef SVCDATAList::iterator SVCDATAListIter;

    typedef std::multimap<int, PID2ServiceData> PID2SERVICES;
    typedef PID2SERVICES::const_iterator PID2SERVICESConstIter;
    typedef PID2SERVICES::iterator PID2SERVICESIter;
    typedef std::pair<PID2SERVICESIter,PID2SERVICESIter> PID2SERVICESIterPair;

    PID2SERVICES pid2services_;

    typedef std::set<int> PIDS;
    typedef PIDS::const_iterator PIDSConstIter;
    typedef PIDS::iterator PIDSIter;
    PIDS pids_;

    int maxprocnum_;
    int registered_services_;
    int maxstartups_;
    Server::MultiplexMode mpmode_;

    // signal handling
    static void handler(int);
    static MultiProcMSDatagramServerPerProcess *object_;
};

class MultiProcMSStreamServerPerProcess: public Server
{
public:
    // ctors and dtor
    MultiProcMSStreamServerPerProcess(int procnum=1, int maxstartups=2, Server::MultiplexMode mpmode = Server::Select);
    virtual ~MultiProcMSStreamServerPerProcess();

    // register service and handlers
    // form of service is address:service:protocol
    // address: X.X.X.X, localhost, N.N.N.N
    // service: port or service name
    // protocol: udp or datagram
    //
    void registerService(const std::string &service, MultiProcMSStreamServer::Handler &handler);

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    MultiProcMSStreamServerPerProcess(const MultiProcMSStreamServerPerProcess &src);
    MultiProcMSStreamServerPerProcess &operator=(const MultiProcMSStreamServerPerProcess &rhs);

protected:
    // track services
    typedef std::list<std::pair<std::string,
                      MultiProcMSStreamServer::Handler * > > SVCSList;
    typedef SVCSList::const_iterator SVCSListConstIter;
    typedef SVCSList::iterator SVCSListIter;
    typedef std::map<const std::string, MultiProcMSStreamServer::Handler * > SVCS;
    typedef SVCS::const_iterator SVCSConstIter;
    typedef SVCS::iterator SVCSIter;
    SVCS services_;

    // track service pids
    struct PID2ServiceData {
        int startups_;
        int maxstartups_;
        std::string uri_;
        std::string service_;
        std::string host_;
        std::string protocol_;
        int port_;
        MultiProcMSStreamServer::Handler *handler_;

        PID2ServiceData(): 
            startups_(0), maxstartups_(-1), uri_(),
            service_(), host_(), protocol_(), port_(-1),
            handler_(NULL) { }
        PID2ServiceData(const PID2ServiceData &pd): 
            startups_(pd.startups_), maxstartups_(pd.maxstartups_), 
            uri_(pd.uri_), service_(pd.service_), host_(pd.host_),
            protocol_(pd.protocol_), port_(pd.port_), handler_(pd.handler_) { }
        PID2ServiceData(int startups, int maxstartups, const std::string &uri, 
                const std::string &service, const std::string &host, 
                const std::string &protocol, int port,
                MultiProcMSStreamServer::Handler *handler):
                startups_(startups), maxstartups_(maxstartups), 
                uri_(uri), service_(service), host_(host),
                protocol_(protocol), port_(port), handler_(handler) { }
        PID2ServiceData &operator=(const PID2ServiceData &rhs) {
            if (this != &rhs)
            {
                startups_ = rhs.startups_;
                maxstartups_ = rhs.maxstartups_;
                uri_ = rhs.uri_;
                service_ = rhs.service_;
                host_ = rhs.host_;
                protocol_ = rhs.protocol_;
                port_ = rhs.port_;
                handler_ = rhs.handler_;
            }
            return(*this);
        }
    };

    typedef std::list<PID2ServiceData> SVCDATAList;
    typedef SVCDATAList::const_iterator SVCDATAListConstIter;
    typedef SVCDATAList::iterator SVCDATAListIter;

    typedef std::multimap<int, PID2ServiceData> PID2SERVICES;
    typedef PID2SERVICES::const_iterator PID2SERVICESConstIter;
    typedef PID2SERVICES::iterator PID2SERVICESIter;
    typedef std::pair<PID2SERVICESIter,PID2SERVICESIter> PID2SERVICESIterPair;

    PID2SERVICES pid2services_;

    typedef std::set<int> PIDS;
    typedef PIDS::const_iterator PIDSConstIter;
    typedef PIDS::iterator PIDSIter;
    PIDS pids_;

    int maxprocnum_;
    int registered_services_;
    int maxstartups_;
    Server::MultiplexMode mpmode_;

    // signal handling
    static void handler(int);
    static MultiProcMSStreamServerPerProcess *object_;
};

}

#endif
