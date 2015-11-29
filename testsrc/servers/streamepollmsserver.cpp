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
#include <servers/IterativeMultiServiceServer.h>

using namespace ombt;

int
main(int argc, char *argv[])
{
    std::string filepath1("/tmp/xxxx");
    std::string filepath2("/tmp/yyyy");

    std::cout << "Creating Stream Socket 1 ..." << std::endl;
    UseCntPtr<StreamSocket> stream_socket1(new StreamSocket());
    assert(stream_socket1->isOk());

    std::cout << "Creating Local Address 1 ... " << filepath1 << std::endl;
    ::unlink(filepath1.c_str());
    UseCntPtr<LocalAddress> my_address1(new LocalAddress(filepath1));
    assert(my_address1->isOk());

    std::cout << "Creating Stream EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(stream_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating Stream Socket 2 ..." << std::endl;
    UseCntPtr<StreamSocket> stream_socket2(new StreamSocket());
    assert(stream_socket2->isOk());

    std::cout << "Creating Local Address 2 ... " << filepath2 << std::endl;
    ::unlink(filepath2.c_str());
    UseCntPtr<LocalAddress> my_address2(new LocalAddress(filepath2));
    assert(my_address2->isOk());

    std::cout << "Creating Stream EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(stream_socket2, my_address2));
    assert(server_ep2->isOk());

    UseCntPtr<IterativeMSStreamServer::Handler> handler1(new IterativeMSStreamServer::Handler);
    UseCntPtr<IterativeMSStreamServer::Handler> handler2(new IterativeMSStreamServer::Handler);

    IterativeMSStreamServer idg(Server::EPoll);
    assert(idg.isOk());

    idg.registerHandler(server_ep1, handler1);
    idg.registerHandler(server_ep2, handler2);

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

