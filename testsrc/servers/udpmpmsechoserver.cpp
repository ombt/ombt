//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "system/Debug.h"
#include "atomic/UseCntPtr.h"
#include "sockets/Socket.h"
#include "sockets/InetAddress.h"
#include "sockets/EndPoint.h"
#include "servers/MultiProcessMultiServiceServer.h"

using namespace ombt;

class MyHandler: public MultiProcMSDatagramServer::Handler {
public:
    MyHandler() { setOk(true); }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *serverep)
    {
        count = BUFSIZ;

        UseCntPtr<EndPoint> pserverep(serverep);
        UseCntPtr<Address> peer(pserverep->getInternalAddress().create());

        assert(pserverep->read(buf, count, *peer) >= 0);
        assert(pserverep->write(buf, count, *peer) >= 0);

        return(1);
    }

    size_t count;
    char buf[BUFSIZ+1];
};

int
main(int argc, char *argv[])
{
    std::string host("localhost");

    assert(argc == 3);

    int port1 = atoi(argv[1]);
    int port2 = atoi(argv[2]);

    std::cout << "Creating UDP Socket 1 ..." << std::endl;
    UseCntPtr<UdpSocket> udp_socket1(new UdpSocket());
    assert(udp_socket1->isOk());

    std::cout << "Creating Inet Address 1 ... " << std::endl;
    UseCntPtr<InetAddress> my_address1(new InetAddress(host, port1));
    assert(my_address1->isOk());

    std::cout << "Creating Datagram EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(udp_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating UDP Socket 2 ..." << std::endl;
    UseCntPtr<UdpSocket> udp_socket2(new UdpSocket());
    assert(udp_socket2->isOk());

    std::cout << "Creating Inet Address 2 ... " << std::endl;
    UseCntPtr<InetAddress> my_address2(new InetAddress(host, port2));
    assert(my_address2->isOk());

    std::cout << "Creating Datagram EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(udp_socket2, my_address2));
    assert(server_ep2->isOk());

    UseCntPtr<MyHandler> handler1(new MyHandler);
    UseCntPtr<MyHandler> handler2(new MyHandler);

    MultiProcMSDatagramServer idg;
    assert(idg.isOk());

    idg.registerHandler(server_ep1, handler1);
    idg.registerHandler(server_ep2, handler2);

    assert(idg.init() == 0);

    assert(idg.run() == 0);;

    assert(idg.finish() == 0);

    return(0);
}
