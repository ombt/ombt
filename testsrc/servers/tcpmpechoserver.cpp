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
#include "servers/MultiProcessServer.h"

using namespace ombt;

class MyHandler: public MultiProcessStreamServer::Handler {
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
            if (errno == ECONNRESET) return(1);
            DUMP(errno);
            return(-1);
        }
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

    std::cout << "Creating TCP Socket ..." << std::endl;
    UseCntPtr<TcpSocket> tcp_socket(new TcpSocket);
    assert(tcp_socket->isOk());

    std::cout << "Creating IPv4 Address ..." << std::endl;
    UseCntPtr<InetAddress> my_address(new InetAddress(host, port));
    assert(my_address->isOk());

    std::cout << "Creating TCP EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(tcp_socket, my_address));
    assert(server_ep->isOk());

    UseCntPtr<MyHandler> handler(new MyHandler);;
    MultiProcessStreamServer iss(server_ep, handler);

    iss.init();
    iss.run();
    iss.finish();

    return(0);
}
