//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <iostream>
#include "system/Debug.h"
#include "atomic/UseCntPtr.h"
#include "sockets/Socket.h"
#include "sockets/InetAddress.h"
#include "sockets/EndPoint.h"
#include "servers/IterativeServerPerProcess.h"
#include "logging/Logging.h"

using namespace ombt;

class MyHandler: public IterativeStreamServer::Handler {
public:
    MyHandler() { setOk(true); }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *clientep)
    {
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

        UseCntPtr<EndPoint> pclientep(clientep);
        count = pclientep->read(buf, count);
        if (count > 0)
        {
            if (pclientep->write(buf, count) > 0)
                return(0);
            else
            {
                DUMP(errno);
                return(-1);
            }
        }
        else if (count == 0)
            return(1);
        else
        {
            DUMP(errno);
            return(-1);
        }
    }
};

int
main(int argc, char *argv[])
{
    std::string host("localhost");

    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " maxstartups host:service:protocol [host:service:protocol ...]" << std::endl;
        return(2);
    }

    dbgopen(stderr);

    int maxstartups = atoi(argv[1]);
    assert(maxstartups > 0);

    IterativeStreamServerPerProcess iss(maxstartups);

    for (int arg=2; arg<argc; ++arg)
    {
        std::cout << "Registering service ..." << argv[arg] << std::endl;
        UseCntPtr<MyHandler> phandler(new MyHandler);;
        iss.registerService(argv[arg], phandler);
    }

    iss.init();
    iss.run();
    iss.finish();

    return(0);
}
