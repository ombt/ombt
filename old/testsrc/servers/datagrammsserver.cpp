
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeMultiServiceServer.h>

using namespace ombt;

int
main(int argc, char *argv[])
{
    std::string filepath1("/tmp/xxxx");
    std::string filepath2("/tmp/xxxx");

    std::cout << "Creating Datagram Socket 1 ..." << std::endl;
    DatagramSocket *datagram_socket1 = new DatagramSocket();
    assert(datagram_socket1->isOk());

    std::cout << "Creating Local Address 1 ..." << std::endl;
    ::unlink(filepath1.c_str());
    LocalAddress *my_address1 = new LocalAddress(filepath1);
    assert(my_address1->isOk());

    std::cout << "Creating Datagram EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(datagram_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating Datagram Socket 2 ..." << std::endl;
    DatagramSocket *datagram_socket2 = new DatagramSocket();
    assert(datagram_socket2->isOk());

    std::cout << "Creating Local Address 2 ..." << std::endl;
    ::unlink(filepath2.c_str());
    LocalAddress *my_address2 = new LocalAddress(filepath2);
    assert(my_address2->isOk());

    std::cout << "Creating Datagram EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(datagram_socket2, my_address2));
    assert(server_ep2->isOk());

    IterativeMSDatagramServer::Handler handler1;
    IterativeMSDatagramServer::Handler handler2;

    IterativeMSDatagramServer idg;
    assert(idg.isOk());

    idg.registerHandler(*server_ep1, handler1);
    idg.registerHandler(*server_ep2, handler2);

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

