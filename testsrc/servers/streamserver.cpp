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
#include "sockets/LocalAddress.h"
#include "sockets/EndPoint.h"
#include "servers/IterativeServer.h"

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
    UseCntPtr<StreamSocket> stream_socket(new StreamSocket());
    assert(stream_socket->isOk());

    std::cout << "Creating Local Address ..." << std::endl;
    ::unlink(filepath.c_str());
    UseCntPtr<LocalAddress> my_address(new LocalAddress(filepath));
    assert(my_address->isOk());

    std::cout << "Creating Stream EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(stream_socket, my_address));
    assert(server_ep->isOk());

    UseCntPtr<IterativeStreamServer::Handler> iss_handler(new IterativeStreamServer::Handler);

    IterativeStreamServer iss(server_ep, iss_handler);

    iss.init();

    iss.run();

    iss.finish();

    return(0);
}

