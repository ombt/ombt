
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/InetAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeServerPerProcess.h>
#include <logging/Logging.h>

using namespace ombt;

class MyHandler: public IterativeDatagramServer::Handler {
public:
    MyHandler() { ok_ = true; }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint &serverep, Address &peer, 
                           void *buf, int count)
    {
        int status = serverep.write(buf, count, peer);
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
    std::string host("localhost");
    int port = 0;

    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " maxstartups host:service:protocol [host:service:protocol ...]" << std::endl;
        return(2);
    }

    dbgopen(stderr);

    int maxstartups = atoi(argv[1]);
    assert(maxstartups > 0);

    MyHandler idghandler;
    IterativeDatagramServerPerProcess idg(maxstartups);
    assert(idg.isOk());

    for (int arg=2; arg<argc; ++arg)
    {
        std::cout << "Registering service ... " << argv[arg] << std::endl;
        idg.registerService(argv[arg], idghandler);
    }

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}
