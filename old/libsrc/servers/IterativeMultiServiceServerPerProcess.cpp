// iterative server class

// headers
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <parsing/Tokenizer.h>
#include <logging/Logging.h>
#include <IterativeMultiServiceServerPerProcess.h>

namespace ombt {

// class statics
IterativeMSDatagramServerPerProcess
    *IterativeMSDatagramServerPerProcess::object_ = NULL;

void
ombt::IterativeMSDatagramServerPerProcess::handler(int sig)
{
    if (object_ != NULL) object_->finish();
    exit(0);
}

// ctors and dtor
IterativeMSDatagramServerPerProcess::IterativeMSDatagramServerPerProcess(int maxprocnum, int maxs, Server::MultiplexMode mpmode):
    Server(), services_(), pid2services_(), pids_(), maxprocnum_(maxprocnum), registered_services_(0), maxstartups_(maxs), mpmode_(mpmode)
{
    MustBeTrue(maxprocnum_ > 0);
    MustBeTrue(maxstartups_ > 0);
    ok_ = true;
}

IterativeMSDatagramServerPerProcess::~IterativeMSDatagramServerPerProcess()
{
    ok_ = false;
}

// register services and handlers
void
IterativeMSDatagramServerPerProcess::registerService(
    const std::string &service, 
    IterativeMSDatagramServer::Handler &handler)
{
    services_.insert(std::pair<std::string,IterativeMSDatagramServer::Handler * >(service, &handler));
}

// operations
int
IterativeMSDatagramServerPerProcess::init()
{
    if (!ok_) return(-1);

    return(0);
}

int
IterativeMSDatagramServerPerProcess::run()
{
    if (!ok_) return(-1);
    if (services_.empty()) return(-1);

    IterativeMSDatagramServerPerProcess::object_ = this;
    ::signal(SIGINT, IterativeMSDatagramServerPerProcess::handler);
    ::signal(SIGQUIT, IterativeMSDatagramServerPerProcess::handler);
    ::signal(SIGTERM, IterativeMSDatagramServerPerProcess::handler);

    SVCSIter sit = services_.begin();
    SVCSIter sitend = services_.end();

    registered_services_ = services_.size();
    int services_per_proc = registered_services_/maxprocnum_;
    if (registered_services_%maxprocnum_ > 0) ++services_per_proc;

    for (int iproc=1; (iproc<=maxprocnum_) && (sit != sitend); ++iproc)
    {
        SVCSList proc_svcs;
        for (int iservice=1; 
            (iservice<=services_per_proc) &&
            (sit != sitend); ++sit, ++iservice)
        {
            dbgprintf("Starting service %s in process %d.\n", 
                      sit->first.c_str(), iproc);
            proc_svcs.push_back(
                std::pair<std::string, IterativeMSDatagramServer::Handler * >(
                    sit->first, sit->second));
        }

        int pid = ::fork();
        if (pid == 0)
        {
            // child server
            IterativeMSDatagramServer server(mpmode_);
            assert(server.isOk());

            SVCSListConstIter psit = proc_svcs.begin();
            SVCSListConstIter psitend = proc_svcs.end();
            
            for ( ; psit != psitend; ++psit)
            {
                std::string uri(psit->first);
                IterativeMSDatagramServer::Handler *hdlr = psit->second;

                Service s(uri);
                MustBeTrue(s.isOk());
                int port = s.getPort();
                std::string service = s.getService();
                std::string host = s.getHost();
                std::string protocol = s.getProtocol();

                // child server
                Socket *socket;
                Address *address;
                if (protocol == "udp")
                {
                    socket = new UdpSocket();
                    MustBeTrue(socket->isOk());
                    address = new InetAddress(host, port);
                    MustBeTrue(address->isOk());
                }
                else if (protocol == "datagram")
                {
                    socket = new DatagramSocket();
                    MustBeTrue(socket->isOk());
                    ::unlink(service.c_str());
                    address = new LocalAddress(service);
                    MustBeTrue(address->isOk());
                }
                else
                {
                    MustBeTrue(0);
                }

                UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
                MustBeTrue(server_ep->isOk());

                MustBeTrue(server.registerHandler(*server_ep, *hdlr) == 0);
            }

            MustBeTrue(server.init() == 0);
            MustBeTrue(server.run() == 0);
            MustBeTrue(server.finish() == 0);

            exit(0);
        }
        else if (pid > 0)
        {
            // parent -- track pid
            SVCSListConstIter psit = proc_svcs.begin();
            SVCSListConstIter psitend = proc_svcs.end();
            
            for ( ; psit != psitend; ++psit)
            {
                std::string uri(psit->first);

                Service s(uri);
                MustBeTrue(s.isOk());

                int port = s.getPort();
                std::string service = s.getService();
                std::string host = s.getHost();
                std::string protocol = s.getProtocol();

                IterativeMSDatagramServer::Handler *hdlr = psit->second;

                dbgprintf("Service %s (pid=%d) started.\n", uri.c_str(), pid);

                PID2ServiceData data(1,maxstartups_,uri,service,host,protocol,port,hdlr);
                pid2services_.insert(std::pair<int, PID2ServiceData>(pid, data));
            }
            pids_.insert(pid);
        }
        else
        {
            // some type of error
            MustBeTrue(pid >= 0);
        }
    }

    for (int status; !pids_.empty(); )
    {
        int status;
        pid_t pid = ::waitpid(-1, &status, 0);
        if (errno == ECHILD)
        {
            pids_.clear();
            return(-1);
        }

        PID2SERVICESIterPair piters = pid2services_.equal_range(pid);
        if (piters.first == piters.second) continue;

        dbgprintf("Process %d died.\n", pid);

        SVCDATAList proc_svcs_data;
        PID2SERVICESIter lbit = piters.first;
        PID2SERVICESIter ubit = piters.second;

        for ( ; lbit != ubit; ++lbit)
        {
            dbgprintf("Service %s (pid=%d) died.\n",
                      lbit->second.uri_.c_str(), pid);
            if (lbit->second.startups_ < lbit->second.maxstartups_)
            {
                dbgprintf("Service %s (pid=%d) attempting to restart (%d out of %d).\n",
                          lbit->second.uri_.c_str(), pid, 
                          lbit->second.startups_, lbit->second.maxstartups_);

                PID2ServiceData pdata(lbit->second.startups_+1,
                                      lbit->second.maxstartups_,
                                      lbit->second.uri_,
                                      lbit->second.service_,
                                      lbit->second.host_,
                                      lbit->second.protocol_,
                                      lbit->second.port_,
                                      lbit->second.handler_);
                proc_svcs_data.push_back(pdata);
            }
            else
            {
                dbgprintf("Service %s (pid=%d) NOT restarted (%d out of %d).\n",
                          lbit->second.uri_.c_str(), pid, 
                          lbit->second.startups_, lbit->second.maxstartups_);
            }
        }

        if (proc_svcs_data.empty())
        {
            dbgprintf("Process %d NOT restarted since there are no services to restart.\n", pid);
            pids_.erase(pid);
            pid2services_.erase(pid);
            continue;
        }

        int childpid = ::fork();
        if (childpid == 0)
        {
            // child server
            IterativeMSDatagramServer server(mpmode_);
            assert(server.isOk());

            SVCDATAListIter psdit = proc_svcs_data.begin();
            SVCDATAListIter psditend = proc_svcs_data.end();
            
            for ( ; psdit != psditend; ++psdit)
            {
                // child server
                Socket *socket;
                Address *address;
                if (psdit->protocol_ == "udp")
                {
                    socket = new UdpSocket();
                    MustBeTrue(socket->isOk());
                    address = new InetAddress(psdit->host_, psdit->port_);
                    MustBeTrue(address->isOk());
                }
                else if (psdit->protocol_ == "datagram")
                {
                    socket = new DatagramSocket();
                    MustBeTrue(socket->isOk());
                    ::unlink(psdit->service_.c_str());
                    address = new LocalAddress(psdit->service_);
                    MustBeTrue(address->isOk());
                }
                else
                {
                    MustBeTrue(0);
                }

                UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
                MustBeTrue(server_ep->isOk());

                MustBeTrue(server.registerHandler(*server_ep, *psdit->handler_) == 0);
            }

            MustBeTrue(server.init() == 0);
            MustBeTrue(server.run() == 0);
            MustBeTrue(server.finish() == 0);

            exit(0);
        }
        else if (childpid > 0)
        {
            // parent -- track pid
            SVCDATAListIter pdit = proc_svcs_data.begin();
            SVCDATAListIter pditend = proc_svcs_data.end();

            for ( ; pdit != pditend; ++pdit)
            {
                dbgprintf("Service %s (pid=%d) restarted (%d out of %d).\n",
                      pdit->uri_.c_str(), childpid, 
                      pdit->startups_, pdit->maxstartups_ );
                pid2services_.insert(std::pair<int, PID2ServiceData>(childpid, *pdit));
                pids_.insert(childpid);
            }
        }
        else
        {
            // some type of error
            MustBeTrue(childpid >= 0);
        }
    }

    return(0);
}

int
IterativeMSDatagramServerPerProcess::finish()
{
    if (!ok_) return(-1);
    if (pids_.empty()) return(0);

    PIDSIter pit = pids_.begin();
    PIDSIter pitend = pids_.end();

    ::signal(SIGCHLD, SIG_IGN);

    for ( ; pit != pitend; ++pit)
    {
        pid_t pid = *pit;
        ::kill(pid, SIGINT);
    }

    pids_.clear();

    return(0);
}

// class statics
IterativeMSStreamServerPerProcess
    *IterativeMSStreamServerPerProcess::object_ = NULL;

void
IterativeMSStreamServerPerProcess::handler(int sig)
{
    if (object_ != NULL) object_->finish();
    exit(0);
}

// ctors and dtor
IterativeMSStreamServerPerProcess::IterativeMSStreamServerPerProcess(int maxprocnum, int maxs, Server::MultiplexMode mpmode):
    Server(), services_(), pid2services_(), pids_(), maxprocnum_(maxprocnum), registered_services_(0), maxstartups_(maxs), mpmode_(mpmode)
{
    MustBeTrue(maxprocnum_ > 0);
    MustBeTrue(maxstartups_ > 0);
    ok_ = true;
}

IterativeMSStreamServerPerProcess::~IterativeMSStreamServerPerProcess()
{
    ok_ = false;
}

// register services and handlers
void
IterativeMSStreamServerPerProcess::registerService(
    const std::string &service, 
    IterativeMSStreamServer::Handler &handler)
{
    services_.insert(std::pair<std::string,IterativeMSStreamServer::Handler * >(service, &handler));
}

// operations
int
IterativeMSStreamServerPerProcess::init()
{
    if (!ok_) return(-1);

    return(0);
}

int
IterativeMSStreamServerPerProcess::run()
{
    if (!ok_) return(-1);
    if (services_.empty()) return(-1);

    IterativeMSStreamServerPerProcess::object_ = this;
    ::signal(SIGINT, IterativeMSStreamServerPerProcess::handler);
    ::signal(SIGQUIT, IterativeMSStreamServerPerProcess::handler);
    ::signal(SIGTERM, IterativeMSStreamServerPerProcess::handler);

    SVCSIter sit = services_.begin();
    SVCSIter sitend = services_.end();

    registered_services_ = services_.size();
    int services_per_proc = registered_services_/maxprocnum_;
    if (registered_services_%maxprocnum_ > 0) ++services_per_proc;

    for (int iproc=1; (iproc<=maxprocnum_) && (sit != sitend); ++iproc)
    {
        SVCSList proc_svcs;
        for (int iservice=1; 
            (iservice<=services_per_proc) &&
            (sit != sitend); ++sit, ++iservice)
        {
            dbgprintf("Starting service %s in process %d.\n", 
                      sit->first.c_str(), iproc);
            proc_svcs.push_back(
                std::pair<std::string, IterativeMSStreamServer::Handler * >(
                    sit->first, sit->second));
        }

        int pid = ::fork();
        if (pid == 0)
        {
            // child server
            IterativeMSStreamServer server(mpmode_);
            assert(server.isOk());

            SVCSListConstIter psit = proc_svcs.begin();
            SVCSListConstIter psitend = proc_svcs.end();
            
            for ( ; psit != psitend; ++psit)
            {
                std::string uri(psit->first);
                IterativeMSStreamServer::Handler *hdlr = psit->second;

                Service s(uri);
                MustBeTrue(s.isOk());
                int port = s.getPort();
                std::string service = s.getService();
                std::string host = s.getHost();
                std::string protocol = s.getProtocol();

                // child server
                Socket *socket;
                Address *address;
                if (protocol == "tcp")
                {
                    socket = new TcpSocket();
                    MustBeTrue(socket->isOk());
                    address = new InetAddress(host, port);
                    MustBeTrue(address->isOk());
                }
                else if (protocol == "stream")
                {
                    socket = new StreamSocket();
                    MustBeTrue(socket->isOk());
                    ::unlink(service.c_str());
                    address = new LocalAddress(service);
                    MustBeTrue(address->isOk());
                }
                else
                {
                    MustBeTrue(0);
                }

                UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
                MustBeTrue(server_ep->isOk());

                MustBeTrue(server.registerHandler(*server_ep, *hdlr) == 0);
            }

            MustBeTrue(server.init() == 0);
            MustBeTrue(server.run() == 0);
            MustBeTrue(server.finish() == 0);

            exit(0);
        }
        else if (pid > 0)
        {
            // parent -- track pid
            SVCSListConstIter psit = proc_svcs.begin();
            SVCSListConstIter psitend = proc_svcs.end();
            
            for ( ; psit != psitend; ++psit)
            {
                std::string uri(psit->first);

                Service s(uri);
                MustBeTrue(s.isOk());

                int port = s.getPort();
                std::string service = s.getService();
                std::string host = s.getHost();
                std::string protocol = s.getProtocol();

                IterativeMSStreamServer::Handler *hdlr = psit->second;

                dbgprintf("Service %s (pid=%d) started.\n", uri.c_str(), pid);

                PID2ServiceData data(1,maxstartups_,uri,service,host,protocol,port,hdlr);
                pid2services_.insert(std::pair<int, PID2ServiceData>(pid, data));
            }
            pids_.insert(pid);
        }
        else
        {
            // some type of error
            MustBeTrue(pid >= 0);
        }
    }

    for (int status; !pids_.empty(); )
    {
        int status;
        pid_t pid = ::waitpid(-1, &status, 0);
        if (errno == ECHILD)
        {
            pids_.clear();
            return(-1);
        }

        PID2SERVICESIterPair piters = pid2services_.equal_range(pid);
        if (piters.first == piters.second) continue;

        dbgprintf("Process %d died.\n", pid);

        SVCDATAList proc_svcs_data;
        PID2SERVICESIter lbit = piters.first;
        PID2SERVICESIter ubit = piters.second;

        for ( ; lbit != ubit; ++lbit)
        {
            dbgprintf("Service %s (pid=%d) died.\n",
                      lbit->second.uri_.c_str(), pid);
            if (lbit->second.startups_ < lbit->second.maxstartups_)
            {
                dbgprintf("Service %s (pid=%d) attempting to restart (%d out of %d).\n",
                          lbit->second.uri_.c_str(), pid, 
                          lbit->second.startups_, lbit->second.maxstartups_);

                PID2ServiceData pdata(lbit->second.startups_+1,
                                      lbit->second.maxstartups_,
                                      lbit->second.uri_,
                                      lbit->second.service_,
                                      lbit->second.host_,
                                      lbit->second.protocol_,
                                      lbit->second.port_,
                                      lbit->second.handler_);
                proc_svcs_data.push_back(pdata);
            }
            else
            {
                dbgprintf("Service %s (pid=%d) NOT restarted (%d out of %d).\n",
                          lbit->second.uri_.c_str(), pid, 
                          lbit->second.startups_, lbit->second.maxstartups_);
            }
        }

        if (proc_svcs_data.empty())
        {
            dbgprintf("Process %d NOT restarted since there are no services to restart.\n", pid);
            pids_.erase(pid);
            pid2services_.erase(pid);
            continue;
        }

        int childpid = ::fork();
        if (childpid == 0)
        {
            // child server
            IterativeMSStreamServer server(mpmode_);
            assert(server.isOk());

            SVCDATAListIter psdit = proc_svcs_data.begin();
            SVCDATAListIter psditend = proc_svcs_data.end();
            
            for ( ; psdit != psditend; ++psdit)
            {
                // child server
                Socket *socket;
                Address *address;
                if (psdit->protocol_ == "tcp")
                {
                    socket = new TcpSocket();
                    MustBeTrue(socket->isOk());
                    address = new InetAddress(psdit->host_, psdit->port_);
                    MustBeTrue(address->isOk());
                }
                else if (psdit->protocol_ == "stream")
                {
                    socket = new StreamSocket();
                    MustBeTrue(socket->isOk());
                    ::unlink(psdit->service_.c_str());
                    address = new LocalAddress(psdit->service_);
                    MustBeTrue(address->isOk());
                }
                else
                {
                    MustBeTrue(0);
                }

                UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
                MustBeTrue(server_ep->isOk());

                MustBeTrue(server.registerHandler(*server_ep, *psdit->handler_) == 0);
            }

            MustBeTrue(server.init() == 0);
            MustBeTrue(server.run() == 0);
            MustBeTrue(server.finish() == 0);

            exit(0);
        }
        else if (childpid > 0)
        {
            // parent -- track pid
            SVCDATAListIter pdit = proc_svcs_data.begin();
            SVCDATAListIter pditend = proc_svcs_data.end();

            for ( ; pdit != pditend; ++pdit)
            {
                dbgprintf("Service %s (pid=%d) restarted (%d out of %d).\n",
                      pdit->uri_.c_str(), childpid, 
                      pdit->startups_, pdit->maxstartups_ );
                pid2services_.insert(std::pair<int, PID2ServiceData>(childpid, *pdit));
                pids_.insert(childpid);
            }
        }
        else
        {
            // some type of error
            MustBeTrue(childpid >= 0);
        }
    }

    return(0);
}

int
IterativeMSStreamServerPerProcess::finish()
{
    if (!ok_) return(-1);
    if (pids_.empty()) return(0);

    PIDSIter pit = pids_.begin();
    PIDSIter pitend = pids_.end();

    ::signal(SIGCHLD, SIG_IGN);

    for ( ; pit != pitend; ++pit)
    {
        pid_t pid = *pit;
        ::kill(pid, SIGINT);
    }

    pids_.clear();

    return(0);
}

}
