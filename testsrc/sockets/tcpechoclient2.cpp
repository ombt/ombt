//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "system/Debug.h"
#include "atomic/UseCntPtr.h"
#include "sockets/Socket.h"
#include "sockets/InetAddress.h"
#include "sockets/EndPoint.h"
#include "logging/Logging.h"

using namespace ombt;

int
main(int argc, char *argv[])
{
    dbgopen(stderr);
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

    DBGPRINTF("%s", "Creating TCP Socket ...");
    TcpSocket *tcp_socket = new TcpSocket();
    MustBeTrue(tcp_socket->isOk());

    DBGPRINTF("%s", "Creating IPv4 Server Address ...");
    InetAddress server_address(host, port);
    MustBeTrue(server_address.isOk());

    DBGPRINTF("%s", "Creating TCP EndPoint ...");
    UseCntPtr<EndPoint> connection(new EndPoint(tcp_socket, NULL));
    MustBeTrue(connection->isOk());

    DBGPRINTF("%s", "Connecting to remote server ...");
    MustBeTrue(connection->connect(server_address) >= 0);
    MustBeTrue(connection->isOk());

    char inbuf[BUFSIZ+1];
    char outbuf[BUFSIZ+1];
    size_t count;
    int status;

    for ( ; msgs_to_send > 0; msgs_to_send--)
    {
        DBGPRINTF("\nMessage left to send ... %d", msgs_to_send);

        strcpy(outbuf, "hello world !!!");
        count = strlen(outbuf);
        count += 1;

        DBGPRINTF("Send message ... %s", outbuf);
        MustBeTrue((status = connection->write(outbuf, count)) >= 0);
        DBGPRINTF("Write status ... %d", status);

        inbuf[0] = '\n';
        InetAddress peer_address;
        DBGPRINTF("%s", "Waiting for echo message ...");
        MustBeTrue((status = connection->read(inbuf, count)) >= 0);
        DBGPRINTF("Read status ... %d", status);

        inbuf[BUFSIZ] = '\0';
        DBGPRINTF("Read ... %s", inbuf);
    }

    DBGPRINTF("%s", "Closing socket ...");
    MustBeTrue(connection->close() == 0);

    return(0);
}

