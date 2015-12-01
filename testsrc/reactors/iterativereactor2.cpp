//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// simple test for iterative reactor class

// system headers
#include <string>

// local headers
#include "system/Debug.h"
#include "logging/Logging.h"
#include "atomic/UseCntPtr.h"
#include "timers/Timer.h"
#include "reactors/IterativeReactor.h"

using namespace ombt;

// timer handler
class MyTimerHandler: public IterativeReactor::BaseTimerHandler {
public:
    MyTimerHandler(): IterativeReactor::BaseTimerHandler() { }
    virtual ~MyTimerHandler() { }

    virtual int handler(EndPoint *psep, Timer mytimer) {
        dbgprintf("In Timer Handler !!!\n");
        // returning 1 closes the endpoint.
        return(1);
    }
};

// read handler
class MyEchoHandler: public IterativeReactor::BaseIOHandler {
public:
    MyEchoHandler(): IterativeReactor::BaseIOHandler() { }
    virtual ~MyEchoHandler() { }

    virtual int handler(EndPoint *pserver_ep) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        dbgprintf("In Echo Handler, socket is %d\n", psep->getSocket());
        char buf[1024];
        size_t count = 1024;
        int status = psep->read(buf, count);
        if (status == 0)
            return(1);
        else if (status < 0)
            return(-1);
        count = status;
        status = psep->write(buf, count);
        if (status == 0)
            return(1);
        else if (status < 0)
            return(-1);
        else
            return(0);
    }
};

// accept handler
class MyAcceptHandler: public IterativeReactor::BaseAcceptHandler {
public:
    MyAcceptHandler(): IterativeReactor::BaseAcceptHandler() { }
    virtual ~MyAcceptHandler() { }

    virtual int handler(EndPoint *pserver_ep, EndPoint *paccept_ep) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        UseCntPtr<EndPoint> paep(paccept_ep);
        dbgprintf("In Accept Handler, listen socket is %d, client socket is %d\n", psep->getSocket(), paep->getSocket());
        MustBeTrue(pserver_->registerEndPoint(paep) == OK);
        UseCntPtr<IterativeReactor::BaseIOHandler> pbioh(new MyEchoHandler);
        MustBeTrue(pserver_->registerReadHandler(paep, pbioh) == OK);
        return(0);
    }
};

// main entry point
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 3)
    {
        dbgprintf("usage: %s seconds port\n", argv[0]);
        return(2);
    }

    int arg = 0;
    int seconds = atoi(argv[++arg]);
    int port = atoi(argv[++arg]);

    dbgprintf("Create an IterativeReactor ...\n");
    UseCntPtr<IterativeReactor> preactor(new IterativeReactor());
    MustBeTrue(preactor != NULL);

    dbgprintf("Create a TCP socket ...\n");
    UseCntPtr<Socket> ptcpsocket(new TcpSocket());
    MustBeTrue(ptcpsocket->isOk());

    dbgprintf("Create an INET address ...\n");
    UseCntPtr<InetAddress> paddress(new InetAddress("localhost", port));
    MustBeTrue(paddress->isOk());

    dbgprintf("Create an EndPoint ...\n");
    UseCntPtr<EndPoint> pendpoint(new EndPoint(ptcpsocket, paddress));
    MustBeTrue(pendpoint->isOk());

    dbgprintf("Bind EndPoint ...\n");
    MustBeTrue(pendpoint->bind() == OK);

    dbgprintf("Listen on EndPoint ...\n");
    MustBeTrue(pendpoint->listen() == OK);

    dbgprintf("Create Accept Handler ...\n");
    UseCntPtr<IterativeReactor::BaseAcceptHandler> pbah(new MyAcceptHandler);

    dbgprintf("Register EndPoint ...\n");
    MustBeTrue(preactor->registerEndPoint(pendpoint) == OK);

    dbgprintf("Register accept handler ...\n");
    MustBeTrue(preactor->registerAcceptHandler(pendpoint, pbah) == OK);

    dbgprintf("Create a %d second timer...\n", seconds);
    Timer tmout(seconds, 0);

    dbgprintf("Create Timer Handler ...\n");
    UseCntPtr<IterativeReactor::BaseTimerHandler> pbth(new MyTimerHandler);

    dbgprintf("Schedule a timer ...\n");
    MustBeTrue(preactor->scheduleTimer(pendpoint, tmout, pbth) == OK);

    dbgprintf("Run init() ...\n");
    MustBeTrue(preactor->init() == OK);

    dbgprintf("Run run() ...\n");
    MustBeTrue(preactor->run() == OK);

    dbgprintf("Run finish() ...\n");
    MustBeTrue(preactor->finish() == OK);

    return(0);
}

