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

using namespace ombt;

int
main(int argc, char *argv[])
{
    std::string host("localhost");
    int port = 0;
    int msgs_to_send = 0;

    if (argc == 3)
    {
        port = atoi(argv[1]);
        msgs_to_send = atoi(argv[2]);
    }
    else if (argc == 4)
    {
        host = argv[1];
        port = atoi(argv[2]);
        msgs_to_send = atoi(argv[3]);
    }
    else
    {
        std::cout << "usage: " << argv[0] << " [host] port number_of_msgs" << std::endl;
        return(2);
    }

    std::cout << "Creating UDP Socket ..." << std::endl;
    UdpSocket *udp_socket = new UdpSocket();
    assert(udp_socket->isOk());

    std::cout << "Creating IPv4 Server Address ..." << std::endl;
    InetAddress *server_address = new InetAddress(host, port);
    assert(server_address->isOk());

    std::cout << "Creating Udp EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> connection(new EndPoint(udp_socket, NULL));
    assert(connection->isOk());

    char inbuf[BUFSIZ+1];
    char outbuf[BUFSIZ+1];
    size_t count;
    int status;

    for ( ; msgs_to_send > 0; msgs_to_send--)
    {
        strcpy(outbuf, "hello world !!!");
        count = strlen(outbuf);
        count += 1;

        std::cout << "Send message ... " << outbuf << std::endl;
        assert((status = connection->write(outbuf, count, *server_address)) >= 0);
        std::cout << "Write status ... " << status << std::endl;

        InetAddress peer_address;
        std::cout << "Waiting for echo message ..." << std::endl;
        assert((status = connection->read(inbuf, count, peer_address)) >= 0);
        std::cout << "Read status ... " << status << std::endl;

        inbuf[BUFSIZ] = '\0';
        std::cout << "Read ..." << inbuf << std::endl;
    }

    std::cout << "Closing socket ..." << std::endl;
    assert(connection->close() == 0);

    return(0);
}

