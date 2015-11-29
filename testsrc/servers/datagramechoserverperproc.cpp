//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeServerPerProcess.h>
#include <logging/Logging.h>

using namespace ombt;

class MyHandler: public IterativeDatagramServer::Handler {
public:
    MyHandler() { setOk(true); }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *serverep, Address *peer, 
                           void *buf, int count)
    {
        UseCntPtr<EndPoint> pserverep(serverep);
        UseCntPtr<Address> ppeer(peer);
        int status = serverep->write(buf, count, *ppeer);
        if (status > 0)
            return(0);
        else if (status == 0)
            return(1);
        else
            return(-1);
    }
};

int
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " maxstartups host:service:proto [host:service:proto ...]" << std::endl;
        return(2);
    }

    dbgopen(stderr);

    int maxstartups = atoi(argv[1]);
    MustBeTrue(maxstartups > 0);

    IterativeDatagramServerPerProcess idg(maxstartups);
    MustBeTrue(idg.isOk());

    for (int arg=2; arg<argc; ++arg)
    {
        std::string filepath = argv[arg];

        std::cout << "Registering Service ..." << argv[arg] << std::endl;
        UseCntPtr<MyHandler> pidghandler(new MyHandler);
        idg.registerService(argv[arg], pidghandler);
    }

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

