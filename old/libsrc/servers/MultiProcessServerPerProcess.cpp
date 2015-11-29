// iterative server class

// headers
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <sockets/EndPoint.h>
#include <sockets/Service.h>
#include <MultiProcessServerPerProcess.h>
#include <parsing/Tokenizer.h>
#include <logging/Logging.h>

namespace ombt {

// class statics
MultiProcessDatagramServerPerProcess
    *MultiProcessDatagramServerPerProcess::object_ = NULL;

void
ombt::MultiProcessDatagramServerPerProcess::handler(int sig)
{
    if (object_ != NULL) object_->finish();
    exit(0);
}

// ctors and dtor
MultiProcessDatagramServerPerProcess::MultiProcessDatagramServerPerProcess(int maxs):
    Server(), services_(), pids_(), maxstartups_(maxs)
{
    ok_ = true;
}

MultiProcessDatagramServerPerProcess::~MultiProcessDatagramServerPerProcess()
{
    ok_ = false;
}

// register services and handlers
void
MultiProcessDatagramServerPerProcess::registerService(
    const std::string &service, 
    MultiProcessDatagramServer::Handler &handler)
{
    services_.insert(std::pair<std::string,MultiProcessDatagramServer::Handler * >(service, &handler));
}

// operations
int
MultiProcessDatagramServerPerProcess::init()
{
    if (!ok_) return(-1);

    return(0);
}

int
MultiProcessDatagramServerPerProcess::run()
{
    if (!ok_) return(-1);
    if (services_.empty()) return(-1);

    MultiProcessDatagramServerPerProcess::object_ = this;
    ::signal(SIGINT, MultiProcessDatagramServerPerProcess::handler);
    ::signal(SIGQUIT, MultiProcessDatagramServerPerProcess::handler);
    ::signal(SIGTERM, MultiProcessDatagramServerPerProcess::handler);

    SVCSIter sit = services_.begin();
    SVCSIter sitend = services_.end();

    for ( ; sit != sitend; ++sit)
    {
        std::string uri(sit->first);
        MultiProcessDatagramServer::Handler *hdlr = sit->second;

        Service s(uri);
        if (!s.isOk()) return(-1);
        int port = s.getPort();
        std::string service = s.getService();
        std::string host = s.getHost();
        std::string protocol = s.getProtocol();

        dbgprintf("Starting service %s.\n", uri.c_str());

        int pid = ::fork();
        if (pid == 0)
        {
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
                exit(-1);
            }

            UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
            MustBeTrue(server_ep->isOk());

            MultiProcessDatagramServer server(*server_ep, *hdlr);
            MustBeTrue(server.isOk());

            if (server.init() != 0) exit(-1);
            if (server.run() != 0) exit(-1);
            if (server.finish() != 0) exit(-1);

            exit(0);
        }
        else if (pid > 0)
        {
            // parent -- track pid
            dbgprintf("Service %s (pid=%d) started.\n", uri.c_str(), pid);
            PIDData data(1,maxstartups_,uri,service,host,protocol,port);
            pids_.insert(std::pair<int, PIDData>(pid, data));
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
        PIDSIter pit = pids_.find(pid);
        if (pit != pids_.end())
        {
            dbgprintf("Service %s (pid=%d) died.\n",
                      pit->second.uri_.c_str(), pid);

            if (pit->second.startups_ < pit->second.maxstartups_)
            {
                dbgprintf("Service %s (pid=%d) attempting to restart.\n",
                          pit->second.uri_.c_str(), pid);

                std::string uri(pit->second.uri_);
                SVCSIter sit = services_.find(uri);
                if (sit == services_.end())
                {
                    pids_.erase(pit);
                    continue;
                }

                PIDData newdata(pit->second);
                newdata.startups_ += 1;
                MultiProcessDatagramServer::Handler *hdlr = sit->second;

                pids_.erase(pit);

               int port = newdata.port_;
               std::string host = newdata.host_;
               std::string service = newdata.service_;
               std::string protocol = newdata.protocol_;

               int childpid = ::fork();
               if (childpid == 0)
               {
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
                       exit(-1);
                   }
       
                   UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
                   MustBeTrue(server_ep->isOk());
       
                   MultiProcessDatagramServer server(*server_ep, *hdlr);
                   MustBeTrue(server.isOk());
       
                   if (server.init() != 0) exit(-1);
                   if (server.run() != 0) exit(-1);
                   if (server.finish() != 0) exit(-1);
       
                   exit(0);
               }
               else if (childpid > 0)
               {
                   // parent -- track pid
		   dbgprintf("Service %s (pid=%d) restarted (%d out of %d).\n",
                             uri.c_str(), childpid, 
                             newdata.startups_, newdata.maxstartups_ );
                   pids_.insert(std::pair<int, PIDData>(childpid, newdata));
               }
               else
               {
                   // some type of error
                   MustBeTrue(childpid >= 0);
               }
            }
            else
            {
		   dbgprintf("Service %s (pid=%d) NOT restarted (%d out of %d).\n",
                             pit->second.uri_.c_str(), 
                             pid, 
                             pit->second.startups_, 
                             pit->second.maxstartups_);
                pids_.erase(pit);
            }
        }
    }

    return(0);
}

int
MultiProcessDatagramServerPerProcess::finish()
{
    if (!ok_) return(-1);
    if (pids_.empty()) return(0);

    PIDSIter pit = pids_.begin();
    PIDSIter pitend = pids_.end();

    ::signal(SIGCHLD, SIG_IGN);

    for ( ; pit != pitend; ++pit)
    {
        pid_t pid = pit->first;
        ::kill(pid, SIGINT);
    }

    pids_.clear();

    return(0);
}

// class statics
MultiProcessStreamServerPerProcess
    *MultiProcessStreamServerPerProcess::object_ = NULL;

void
MultiProcessStreamServerPerProcess::handler(int sig)
{
    if (object_ != NULL) object_->finish();
    exit(0);
}

// ctor and dtor
MultiProcessStreamServerPerProcess::MultiProcessStreamServerPerProcess(int maxs):
    Server(), services_(), pids_(), maxstartups_(maxs)
{
    ok_ = true;
}

MultiProcessStreamServerPerProcess::~MultiProcessStreamServerPerProcess()
{
    ok_ = false;
}

void
MultiProcessStreamServerPerProcess::registerService(
    const std::string &service, 
    MultiProcessStreamServer::Handler &handler)
{
    services_.insert(std::pair<std::string,MultiProcessStreamServer::Handler * >(service, &handler));
}

// operations
int
MultiProcessStreamServerPerProcess::init()
{
    if (!ok_) return(-1);
    return(0);
}

int
MultiProcessStreamServerPerProcess::run()
{
    if (!ok_) return(-1);
    if (services_.empty()) return(-1);

    MultiProcessStreamServerPerProcess::object_ = this;
    ::signal(SIGINT, MultiProcessStreamServerPerProcess::handler);
    ::signal(SIGQUIT, MultiProcessStreamServerPerProcess::handler);
    ::signal(SIGTERM, MultiProcessStreamServerPerProcess::handler);

    SVCSIter sit = services_.begin();
    SVCSIter sitend = services_.end();

    for ( ; sit != sitend; ++sit)
    {
        std::string uri(sit->first);
        MultiProcessStreamServer::Handler *hdlr = sit->second;

        Service s(uri);
        if (!s.isOk()) return(-1);
        int port = s.getPort();
        std::string service = s.getService();
        std::string host = s.getHost();
        std::string protocol = s.getProtocol();

        dbgprintf("Starting service %s.\n", uri.c_str());

        int pid = ::fork();
        if (pid == 0)
        {
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
                exit(-1);
            }

            UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
            MustBeTrue(server_ep->isOk());

            MultiProcessStreamServer server(*server_ep, *hdlr);
            MustBeTrue(server.isOk());

            if (server.init() != 0) exit(-1);
            if (server.run() != 0) exit(-1);
            if (server.finish() != 0) exit(-1);

            exit(0);
        }
        else if (pid > 0)
        {
            // parent -- track pid
            dbgprintf("Service %s (pid=%d) started.\n", uri.c_str(), pid);
            PIDData data(1,maxstartups_,uri,service,host,protocol,port);
            pids_.insert(std::pair<int, PIDData>(pid, data));
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
        PIDSIter pit = pids_.find(pid);
        if (pit != pids_.end())
        {
            dbgprintf("Service %s (pid=%d) died.\n",
                      pit->second.uri_.c_str(), pid);
            if (pit->second.startups_ < pit->second.maxstartups_)
            {
                dbgprintf("Service %s (pid=%d) attempting to restart.\n",
                          pit->second.uri_.c_str(), pid);

                std::string uri(pit->second.uri_);
                SVCSIter sit = services_.find(uri);
                if (sit == services_.end())
                {
                    pids_.erase(pit);
                    continue;
                }

                PIDData newdata(pit->second);
                newdata.startups_ += 1;
                MultiProcessStreamServer::Handler *hdlr = sit->second;

                pids_.erase(pit);

               int port = newdata.port_;
               std::string host = newdata.host_;
               std::string service = newdata.service_;
               std::string protocol = newdata.protocol_;

               int childpid = ::fork();
               if (childpid == 0)
               {
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
                       exit(-1);
                   }
       
                   UseCntPtr<EndPoint> server_ep(new EndPoint(socket, address));
                   MustBeTrue(server_ep->isOk());
       
                   MultiProcessStreamServer server(*server_ep, *hdlr);
                   MustBeTrue(server.isOk());
       
                   if (server.init() != 0) exit(-1);
                   if (server.run() != 0) exit(-1);
                   if (server.finish() != 0) exit(-1);
       
                   exit(0);
               }
               else if (childpid > 0)
               {
                   // parent -- track pid
		   dbgprintf("Service %s (pid=%d) restarted (%d out of %d).\n",
                             uri.c_str(), childpid, 
                             newdata.startups_, newdata.maxstartups_ );
                   pids_.insert(std::pair<int, PIDData>(childpid, newdata));
               }
               else
               {
                   // some type of error
                   MustBeTrue(childpid >= 0);
               }
            }
            else
            {
		dbgprintf("Service %s (pid=%d) NOT restarted (%d out of %d).\n",
                          pit->second.uri_.c_str(), 
                          pid, 
                          pit->second.startups_, 
                          pit->second.maxstartups_);
                pids_.erase(pit);
            }
        }
    }

    return(0);
}

int
MultiProcessStreamServerPerProcess::finish()
{
    if (!ok_) return(-1);

    if (pids_.empty()) return(0);

    PIDSIter pit = pids_.begin();
    PIDSIter pitend = pids_.end();

    ::signal(SIGCHLD, SIG_IGN);

    for ( ; pit != pitend; ++pit)
    {
        pid_t pid = pit->first;
        ::kill(pid, SIGINT);
    }

    pids_.clear();

    return(0);
}

}
