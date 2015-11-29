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
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>

using namespace ombt;

int
main(int argc, char *argv[])
{
    std::string host("localhost");
    std::string filepath("/tmp/XXX");

    if (argc == 2)
    {
        filepath = argv[1];
    }
    else
    {
        std::cout << "usage: " << argv[0] << " file_path" << std::endl;
        return(2);
    }

    std::cout << "Creating Streams Socket ..." << std::endl;
    StreamSocket *stream_socket = new StreamSocket();
    assert(stream_socket->isOk());


    std::cout << "Creating Local Address ..." << std::endl;
    ::unlink(filepath.c_str());
    LocalAddress *my_address = new LocalAddress(filepath);
    assert(my_address->isOk());

    std::cout << "Creating Stream EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> connection(new EndPoint(stream_socket, my_address));
    assert(connection->isOk());

    std::cout << "Binding to addresss ..." << std::endl;
    assert(connection->bind() == 0);

    std::cout << "Listening to address ..." << std::endl;
    assert(connection->listen() == 0);

    LocalAddress peer_address;
    char buf[BUFSIZ+1];
    size_t count;

    // echo loop
    while (true)
    {
        int status;
        UseCntPtr<EndPoint> new_client_connection;

        std::cout << std::endl << "Waiting for a message from a new client..." << std::endl;
        bool retry = false;
        new_client_connection = connection->accept(retry);
        assert(new_client_connection.ptr() != NULL);

        count = BUFSIZ;
        assert((status = new_client_connection->read(buf, count)) >= 0);
        std::cout << "Status from read ... " << status << std::endl;
        std::cout << "Read ..." << buf << std::endl;

        buf[BUFSIZ] = '\0';
        count = status;

        assert((status = new_client_connection->write(buf, count)) >= 0);
        std::cout << "Status from write ... " << status << std::endl;
        std::cout << "Wrote ..." << buf << std::endl;

        std::cout << "Closing client socket ..." << std::endl;
        assert(new_client_connection->close() == 0);
    }

    std::cout << "Closing socket ..." << std::endl;
    assert(connection->close() == 0);

    return(0);
}

