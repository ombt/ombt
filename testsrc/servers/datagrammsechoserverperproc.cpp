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
#include <servers/IterativeMultiServiceServerPerProcess.h>
#include <logging/Logging.h>

using namespace ombt;

class MyHandler: public IterativeMSDatagramServer::Handler {
public:
    MyHandler() { setOk(true); }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *serverep)
    {
        count = BUFSIZ;

        UseCntPtr<EndPoint> pserverep(serverep);
        UseCntPtr<Address> peer(pserverep->getInternalAddress().create());

        count = pserverep->read(buf, count, *peer);
        if (count > 0)
        {
            if (pserverep->write(buf, count, *peer) > 0)
                return(0);
            else
                return(-1);
        }
        else if (count < 0)
            return(-1); // error
        else
            return(1); // EOF -- all done
    }

    size_t count;
    char buf[BUFSIZ+1];
};

int
main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "usage: " << argv[0] << " numprocs maxstartups [epoll|select] host:service:proto [host:service:proto ...]" << std::endl;
        return(2);
    }

    dbgopen(stderr);

    int arg=1;
    int numprocs = ::atoi(argv[arg++]);
    assert(numprocs > 0);
    int maxstartups = ::atoi(argv[arg++]);
    assert(maxstartups > 0);

    Server::MultiplexMode mpmode;
    if (std::string("epoll") == argv[arg])
    {
        mpmode = Server::EPoll;
        arg += 1;
    }
    else if (std::string("select") == argv[arg])
    {
        mpmode = Server::Select;
        arg += 1;
    }

    IterativeMSDatagramServerPerProcess idg(numprocs, maxstartups, mpmode);
    assert(idg.isOk());

    for ( ; arg<argc; ++arg)
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

