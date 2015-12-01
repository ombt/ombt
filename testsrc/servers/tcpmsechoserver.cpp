//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "atomic/UseCntPtr.h"
#include "sockets/Socket.h"
#include "sockets/InetAddress.h"
#include "sockets/EndPoint.h"
#include "servers/IterativeMultiServiceServer.h"
#include "logging/Logging.h"

using namespace ombt;

class MyHandler: public IterativeMSStreamServer::Handler {
public:
    MyHandler() { setOk(true); }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *clientep)
    {
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

        UseCntPtr<EndPoint> pclientep(clientep);
        count = pclientep->read(buf, count);
        if (count > 0)
        {
            if (pclientep->write(buf, count) > 0)
                return(0);
            else
            {
                DUMP(errno);
                return(-1);
            }
        }
        else if (count == 0)
            return(1);
        else
        {
            DUMP(errno);
            return(-1);
        }
    }
};

int
main(int argc, char *argv[])
{
    dbgopen(stderr);

    std::string host("localhost");

    if (argc != 3)
    {
        DBGPRINTF("usage: %s port1 port2", argv[0]);
        return(2);
    }

    int port1 = atoi(argv[1]);
    int port2 = atoi(argv[2]);

    DBGPRINTF("%s", "Creating TCP Socket 1 ...");
    UseCntPtr<TcpSocket> tcp_socket1(new TcpSocket());
    assert(tcp_socket1->isOk());

    DBGPRINTF("%s", "Creating INET Address 1 ...");
    UseCntPtr<InetAddress> my_address1(new InetAddress(host, port1));
    assert(my_address1->isOk());

    DBGPRINTF("%s", "Creating TcpSocket EndPoint 1 ...");
    UseCntPtr<EndPoint> server_ep1(new EndPoint(tcp_socket1, my_address1));
    assert(server_ep1->isOk());

    DBGPRINTF("%s", "Creating TcpSocket Socket 2 ...");
    UseCntPtr<TcpSocket> tcp_socket2(new TcpSocket());
    assert(tcp_socket2->isOk());

    DBGPRINTF("%s", "Creating INET Address 2 ...");
    UseCntPtr<InetAddress> my_address2(new InetAddress(host, port2));
    assert(my_address2->isOk());

    DBGPRINTF("%s", "Creating TcpSocket EndPoint 2 ...");
    UseCntPtr<EndPoint> server_ep2(new EndPoint(tcp_socket2, my_address2));
    assert(server_ep2->isOk());

    UseCntPtr<MyHandler> handler1(new MyHandler);
    UseCntPtr<MyHandler> handler2(new MyHandler);

    IterativeMSStreamServer idg;
    assert(idg.isOk());

    idg.registerHandler(server_ep1, handler1);
    idg.registerHandler(server_ep2, handler2);

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

