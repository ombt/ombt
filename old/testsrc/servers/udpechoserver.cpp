
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/InetAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeServer.h>

using namespace ombt;

class MyHandler: public IterativeDatagramServer::Handler {
public:
    MyHandler() { ok_ = true; }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint &serverep, Address &peer, 
                           void *buf, int count)
    {
        int status = serverep.write(buf, count, peer);
        if (status > 0)
            return(0);
        else if (status == 0)
            return(1);
        else
            return(-1);
    }
};

int
main(int argc, char *argv[])
{
    std::string host("localhost");
    int port = 0;

    if (argc == 2)
    {
        port = atoi(argv[1]);
    }
    else if (argc == 3)
    {
        host = argv[1];
        port = atoi(argv[2]);
    }
    else
    {
        std::cout << "usage: " << argv[0] << " [host] port" << std::endl;
        return(2);
    }

    std::cout << "Creating UDP Socket ..." << std::endl;
    UdpSocket *udp_socket = new UdpSocket();
    assert(udp_socket->isOk());

    std::cout << "Creating IPv4 Address ..." << std::endl;
    InetAddress *my_address = new InetAddress(host, port);
    assert(my_address->isOk());

    std::cout << "Creating Udp EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(udp_socket, my_address));
    assert(server_ep->isOk());

    MyHandler idghandler;
    IterativeDatagramServer idg(*server_ep, idghandler);
    assert(idg.isOk());

    idg.init();
    idg.run();
    idg.finish();

    return(0);
}
