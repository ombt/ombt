#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <list>
#include <vector>

#include <Debug.h>
#include <logging/Logging.h>

#include <frameworks/IterativeServer.h>

using namespace ombt;
using namespace std;

class EchoClientHandler: public IterativeServer::BaseIOHandler {
public:
    EchoClientHandler(): IterativeServer::BaseIOHandler() { DBGTRACE(); }
    virtual ~EchoClientHandler() { DBGTRACE(); }
    virtual int handler(EndPoint *pclientep) {
        size_t count = BUFSIZ;
        char buf[BUFSIZ+1];
DBGTRACE();
        UseCntPtr<EndPoint> pcep(pclientep);
DBGTRACE();
dbgprintf("handler: socket = %d\n", pcep->getSocket());
        int cntread = pcep->read(buf, count);
DBGTRACE();
        if (cntread == 0)
        {
DBGTRACE();
            return(1);
        }
        else if (cntread < 0)
        {
DBGTRACE();
            return(-1);
        }
        else
        {
            buf[BUFSIZ] = '\0';
DBGTRACE();
            int status = pcep->write(buf, cntread);
DBGTRACE();
            if (status == 0)
            {
DBGTRACE();
                return(1);
            }
            else if (status < 0)
            {
DBGTRACE();
                return(-1);
            }
            else
            {
DBGTRACE();
                return(0);
            }
        }
    }
};

class AcceptHandler: public IterativeServer::BaseIOHandler {
public:
    AcceptHandler(): IterativeServer::BaseIOHandler() { }
    virtual ~AcceptHandler() { }
    virtual int handler(EndPoint *acceptep) {
        bool retry;
        UseCntPtr<EndPoint> peer(acceptep->accept(retry));
        if (peer.ptr() == NULL)
        {
            if (retry)
                return(0);
            else
                return(1);
        }
        UseCntPtr<EchoClientHandler> pech(new EchoClientHandler);
        pserver_->registerReadHandler(peer, pech);
        return(pech->handler(peer));
    }
};

class EchoServer: public IterativeServer {
public:
    // ctor and dtor
    EchoServer(const std::string &host, int port):
        IterativeServer(), host_(host), port_(port) { }
    ~EchoServer() { pserverep->close(); }

    // override these
    int init() {
        UseCntPtr<TcpSocket> psocket(new TcpSocket);
        MustBeTrue(psocket != NULL && psocket->isOk());

        UseCntPtr<InetAddress> paddress(new InetAddress(host_, port_));
        MustBeTrue(paddress != NULL && paddress->isOk());

        UseCntPtr<EndPoint> pserverep(new EndPoint(psocket, paddress));
        MustBeTrue(pserverep != NULL && pserverep->isOk());

        MustBeTrue(pserverep->bind() == 0);
        MustBeTrue(pserverep->listen() == 0);

        registerReadHandler(pserverep, new AcceptHandler);
        return(0);
    }

private:
    // leper colony
    EchoServer(const EchoServer &);
    EchoServer &operator=(const EchoServer &);

protected:
    // data
    std::string host_;
    int port_;
    UseCntPtr<EndPoint> pserverep;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 3)
    {
        std::cerr << "usage: " << argv[0] << " host port" << std::endl;
        return(2);
    }

    int arg = 0;
    std::string host(argv[++arg]);
    int port = atoi(argv[++arg]);
    dbgprintf("host=%s, port=%d\n", host.c_str(), port);

    EchoServer echoserver(host, port);

    MustBeTrue(echoserver.init() == 0);
    MustBeTrue(echoserver.run() == 0);
    MustBeTrue(echoserver.finish() == 0);

    return(0);
}
