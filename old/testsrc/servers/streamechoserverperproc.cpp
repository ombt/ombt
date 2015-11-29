
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeServerPerProcess.h>
#include <logging/Logging.h>

using namespace ombt;

class MyHandler: public IterativeStreamServer::Handler {
public:
    MyHandler() { ok_ = true; }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint &clientep)
    {
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

        count = clientep.read(buf, count);
        if (count > 0)
        {
            if (clientep.write(buf, count) > 0)
            {
                return(0);
            }
            else
            {
                return(-1);
            }
        }
        else if (count == 0)
        {
            return(1);
        }
        else
        {
            return(-1);
        }
    }
};

int
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " maxstartups host:service:protocol [host:service:protocol ...]" << std::endl;
        return(2);
    }

    dbgopen(stderr);

    int maxstartups = atoi(argv[1]);
    assert(maxstartups > 0);

    MyHandler iss_handler;
    IterativeStreamServerPerProcess iss(maxstartups);
    assert(iss.isOk());

    for (int arg=2; arg<argc; ++arg)
    {
        std::cout << "Registering service ... " << argv[arg] << std::endl;
        iss.registerService(argv[arg], iss_handler);
    }

    iss.init();

    iss.run();

    iss.finish();

    return(0);
}

