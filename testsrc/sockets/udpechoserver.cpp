//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/InetAddress.h>
#include <sockets/EndPoint.h>

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

    std::cout << "Creating UDP Socket ..." << std::endl;
    UdpSocket *udp_socket = new UdpSocket();
    assert(udp_socket->isOk());

    std::cout << "Creating IPv4 Address ..." << std::endl;
    InetAddress *my_address = new InetAddress(host, port);
    assert(my_address->isOk());

    std::cout << "Creating Udp EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> connection(new EndPoint(udp_socket, my_address));
    assert(connection->isOk());

    std::cout << "Binding to addresss ..." << std::endl;
    assert(connection->bind() == 0);

    InetAddress peer_address;
    char buf[BUFSIZ+1];
    size_t count;

    // echo loop
    while (true)
    {
        int status;

        count = BUFSIZ;
        std::cout << "Waiting for a message ..." << std::endl;
        assert((status = connection->read(buf, count, peer_address)) >= 0);
        std::cout << "Status from read ... " << status << std::endl;

        buf[BUFSIZ] = '\0';
        count = status;

        std::cout << "Read ..." << buf << std::endl;
        assert((status = connection->write(buf, count, peer_address)) >= 0);
        std::cout << "Status from write ... " << status << std::endl;
        std::cout << "Wrote ..." << buf << std::endl;
    }

    std::cout << "Closing socket ..." << std::endl;
    assert(connection->close() == 0);

    return(0);
}

