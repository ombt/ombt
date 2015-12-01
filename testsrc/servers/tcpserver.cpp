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
#include "servers/IterativeServer.h"

using namespace ombt;

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
    UseCntPtr<TcpSocket> tcp_socket(new TcpSocket());
    assert(tcp_socket->isOk());

    std::cout << "Creating IPv4 Address ..." << std::endl;
    UseCntPtr<InetAddress> my_address(new InetAddress(host, port));
    assert(my_address->isOk());

    std::cout << "Creating TCP EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(tcp_socket, my_address));
    assert(server_ep->isOk());

    UseCntPtr<IterativeStreamServer::Handler> handler(new IterativeStreamServer::Handler);

    IterativeStreamServer iss(server_ep, handler);

    iss.init();
    iss.run();
    iss.finish();

    return(0);
}

