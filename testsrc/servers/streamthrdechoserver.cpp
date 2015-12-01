//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "system/Debug.h"
#include "atomic/UseCntPtr.h"
#include "sockets/Socket.h"
#include "sockets/LocalAddress.h"
#include "sockets/EndPoint.h"
#include "servers/MultiThreadServer.h"

using namespace ombt;

class MyHandler: public MultiThreadStreamServer::Handler {
public:
    MyHandler() {
TRACE();
        setOk(true);
TRACE();
    }
    virtual ~MyHandler() {
TRACE();
    }

    virtual int operator()(EndPoint *clientep) {
TRACE();
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

TRACE();
        UseCntPtr<EndPoint> pclientep(clientep);
TRACE();
        count = pclientep->read(buf, count);
TRACE();
        if (count > 0)
        {
TRACE();
            if (pclientep->write(buf, count) > 0)
            {
TRACE();
                return(0);
            }
            else
            {
TRACE();
                return(-1);
            }
        }
        else if (count == 0)
        {
TRACE();
            return(1);
        }
        else
        {
TRACE();
            return(-1);
        }
    }
};

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

TRACE();
    std::cout << "Creating Streams Socket ..." << std::endl;
    UseCntPtr<StreamSocket> stream_socket(new StreamSocket());
    assert(stream_socket->isOk());

TRACE();
    std::cout << "Creating Local Address ..." << std::endl;
    ::unlink(filepath.c_str());
    UseCntPtr<LocalAddress> my_address(new LocalAddress(filepath));
    assert(my_address->isOk());

TRACE();
    std::cout << "Creating Stream EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(stream_socket, my_address));
    assert(server_ep->isOk());

TRACE();
    UseCntPtr<MyHandler> iss_handler(new MyHandler);
    MultiThreadStreamServer iss(server_ep, iss_handler);

TRACE();
    iss.init();

TRACE();
    iss.run();

TRACE();
    iss.finish();

TRACE();
    return(0);
}

