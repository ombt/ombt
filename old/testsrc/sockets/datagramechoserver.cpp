
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
    std::string filepath("localhost");

    if (argc == 2)
    {
        filepath = argv[1];
    }
    else
    {
        std::cout << "usage: " << argv[0] << " file_path" << std::endl;
        return(2);
    }

    std::cout << "Creating Datagram Socket ..." << std::endl;
    DatagramSocket *datagram_socket = new DatagramSocket();
    assert(datagram_socket->isOk());

    std::cout << "Creating Local Address ..." << std::endl;
    ::unlink(filepath.c_str());
    LocalAddress *my_address = new LocalAddress(filepath);
    assert(my_address->isOk());

    std::cout << "Creating Datagram EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> connection(new EndPoint(datagram_socket, my_address));
    assert(connection->isOk());

    std::cout << "Binding to addresss ..." << std::endl;
    assert(connection->bind() == 0);

    LocalAddress peer_address;
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

