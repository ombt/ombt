//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <iostream>
#include "system/Debug.h"
#include "atomic/UseCntPtr.h"
#include "sockets/Socket.h"
#include "sockets/InetAddress.h"
#include "sockets/EndPoint.h"
#include "servers/IterativeServer.h"

#include "logging/Logging.h"

using namespace ombt;

class MyHandler: public IterativeStreamServer::Handler {
public:
    MyHandler() { setOk(true); }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *clientep)
    {
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

        DBGTRACE();
        UseCntPtr<EndPoint> pclientep(clientep);
        count = pclientep->read(buf, count);
        DBGPRINTF("read %d bytes ...", count);
        if (count > 0)
        {
            int num;
            if ((num = pclientep->write(buf, count)) > 0)
            {
                DBGPRINTF("wrote %d bytes ...", num);
                return(0);
            }
            else
            {
                DBGPRINTF("write errno %d ...", errno);
                DUMP(errno);
                return(-1);
            }
        }
        else if (count == 0)
        {
            DBGPRINTF("read EOF %d ...", errno);
            return(1);
        }
        else
        {
            DBGPRINTF("read errno %d ...", errno);
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

    std::cout << "Creating TCP Socket ..." << std::endl;
    UseCntPtr<TcpSocket> tcp_socket(new TcpSocket());
    assert(tcp_socket->isOk());

    std::cout << "Creating IPv4 Address ..." << std::endl;
    UseCntPtr<InetAddress> my_address(new InetAddress(host, port));
    assert(my_address->isOk());

    std::cout << "Creating TCP EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(tcp_socket, my_address));
    assert(server_ep->isOk());

    UseCntPtr<MyHandler> handler(new MyHandler);
    IterativeStreamServer iss(server_ep, handler);

    iss.init();
    iss.run();
    iss.finish();

    return(0);
}
