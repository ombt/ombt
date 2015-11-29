
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeServer.h>

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
    UseCntPtr<EndPoint> server_ep(new EndPoint(datagram_socket, my_address));
    assert(server_ep->isOk());

    IterativeDatagramServer::Handler idghandler;
    IterativeDatagramServer idg(*server_ep, idghandler);
    assert(idg.isOk());

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

