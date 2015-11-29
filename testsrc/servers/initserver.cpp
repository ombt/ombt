//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>
#include <Debug.h>
#include <servers/InitServer.h>
#include <logging/Logging.h>

using namespace ombt;

class MyServer: public InitServer::SubServer {
public:
    MyServer() { setOk(true); }
    virtual ~MyServer() { }

    virtual int operator()(const std::string &uri)
    {
        int seconds = ::getpid()%21;
        dbgprintf("child process %s (pid=%d) (sleep=%d) ...\n", uri.c_str(), ::getpid(), seconds);
        ::sleep(seconds);
        return(0);
    }
};

int
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage: " << argv[0] << " maxstartups host:service:protocol [host:service:protocol ...]" << std::endl;
        return(2);
    }

    dbgopen(stderr);

    int maxstartups = atoi(argv[1]);
    assert(maxstartups > 0);

    InitServer iss(maxstartups);
    assert(iss.isOk());

    for (int arg=2; arg<argc; ++arg)
    {
        std::cerr << "Registering service ... " << argv[arg] << std::endl;
        UseCntPtr<MyServer> psubserver(new MyServer);
        iss.registerService(argv[arg], psubserver);
    }

    iss.init();

    iss.run();

    iss.finish();

    return(0);
}

