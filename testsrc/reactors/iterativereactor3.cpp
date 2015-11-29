//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// test for iterative reactor class

// system headers
#include <string>

// local headers
#include <Debug.h>
#include <logging/Logging.h>
#include <atomic/UseCntPtr.h>
#include <timers/Timer.h>
#include <reactors/IterativeReactor.h>
#include <threads/Thread.h>
#include <threads/Task.h>
#include <threads/ThreadBarrier.h>
#include <signals/Signal.h>

using namespace ombt;
using namespace std;

// timer handler
class MyTimerHandler: public IterativeReactor::BaseTimerHandler {
public:
    MyTimerHandler(): IterativeReactor::BaseTimerHandler() { }
    virtual ~MyTimerHandler() { }

    virtual int handler(EndPoint *psep, Timer mytimer) {
        dbgprintf("In Timer Handler !!!\n");
        pserver_->finish();
        return(0);
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

// signal handler
class MySignalHandler: public IterativeReactor::BaseSignalHandler {
public:
    MySignalHandler(): IterativeReactor::BaseSignalHandler() { }
    virtual ~MySignalHandler() { }

    virtual int handler(EndPoint *pserver_ep, int signalno) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        dbgprintf("In Signal Handler, signal is %d\n", signalno);
        pserver_->finish();
        return(0);
    }
};

// server thread task
class ServerTask: public Task {
public:
    ServerTask(int seconds, int port, EndPoint* psep,
               IterativeReactor::BaseSignalHandler *pbsh):
        Task(), seconds_(seconds), port_(port), psep_(psep), pbsh_(pbsh) {
    }
    ~ServerTask() {
    }

    virtual int init() {
        dbgprintf("Create an IterativeReactor ...\n");
        preactor_ = UseCntPtr<IterativeReactor>(new IterativeReactor());
        MustBeTrue(preactor_ != NULL);

        dbgprintf("Create a TCP socket ...\n");
        ptcpsocket_ = UseCntPtr<Socket>(new TcpSocket());
        MustBeTrue(ptcpsocket_->isOk());

        dbgprintf("Create an INET address ...\n");
        paddress_ = UseCntPtr<InetAddress>(new InetAddress("localhost", port_));
        MustBeTrue(paddress_->isOk());

        dbgprintf("Create an EndPoint ...\n");
        pendpoint_ = UseCntPtr<EndPoint>(new EndPoint(ptcpsocket_, paddress_));
        MustBeTrue(pendpoint_->isOk());

        dbgprintf("Bind EndPoint ...\n");
        MustBeTrue(pendpoint_->bind() == OK);

        dbgprintf("Listen on EndPoint ...\n");
        MustBeTrue(pendpoint_->listen() == OK);

        dbgprintf("Create Accept Handler ...\n");
        UseCntPtr<IterativeReactor::BaseAcceptHandler> pbah(new MyAcceptHandler);

        dbgprintf("Register EndPoint ...\n");
        MustBeTrue(preactor_->registerEndPoint(pendpoint_) == OK);

        dbgprintf("Register accept handler ...\n");
        MustBeTrue(preactor_->registerAcceptHandler(pendpoint_, pbah) == OK);

        dbgprintf("Create a %d second timer...\n", seconds_);
        Timer tmout(seconds_, 0);

        dbgprintf("Create Timer Handler ...\n");
        UseCntPtr<IterativeReactor::BaseTimerHandler> pbth(new MyTimerHandler);

        dbgprintf("Schedule a timer ...\n");
        MustBeTrue(preactor_->scheduleTimer(pendpoint_, tmout, pbth) == OK);

        dbgprintf("Register Signal EndPoint ...\n");
        MustBeTrue(preactor_->registerEndPoint(psep_) == OK);

        dbgprintf("Register Signal handler ...\n");
        MustBeTrue(preactor_->registerSignalHandler(psep_, pbsh_) == OK);

        dbgprintf("Run init() ...\n");
        MustBeTrue(preactor_->init() == OK);

        return 0;
    }
    virtual int run() {
        dbgprintf("Run run() ...\n");
        MustBeTrue(preactor_->run() == OK);
        return 0;
    }
    virtual int finish() {
        dbgprintf("Run finish() ...\n");
        MustBeTrue(preactor_->finish() == OK);
        return 0;
    }

private:
    int seconds_;
    int port_;
    UseCntPtr<IterativeReactor> preactor_;
    UseCntPtr<Socket> ptcpsocket_;
    UseCntPtr<InetAddress> paddress_;
    UseCntPtr<EndPoint> pendpoint_;
    UseCntPtr<EndPoint> psep_;
    UseCntPtr<IterativeReactor::BaseSignalHandler> pbsh_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " seconds port" << endl;
        return(2);
    }

    int arg = 0;
    int seconds = atoi(argv[++arg]);
    int port = atoi(argv[++arg]);

    dbgprintf("Create a Signal mask ...\n");
    Signal::Mask sigmask;

    dbgprintf("Set Signal mask to catch SIGINT ...\n");
    sigmask.set(SIGINT);
    dbgprintf("Set Signal mask to catch SIGABRT ...\n");
    sigmask.set(SIGABRT);

    dbgprintf("Create a Signal Socket ...\n");
    UseCntPtr<Socket> psig(new Signal(sigmask));
    MustBeTrue(psig != NULL);

    dbgprintf("Create a Signal EndPoint ...\n");
    UseCntPtr<EndPoint> psep(new SignalEndPoint(psig));
    MustBeTrue(psep != NULL);

    dbgprintf("Create Signal Handler ...\n");
    UseCntPtr<IterativeReactor::BaseSignalHandler> pbsh(new MySignalHandler);

    UseCntPtr<Thread> pserver(new Thread(new ServerTask(seconds, port, psep, pbsh)));
    MustBeTrue(pserver->isOk());
    MustBeTrue(pserver->run() == 0);
    MustBeTrue(pserver->isOk());

    void *retval;
    pserver->join(retval);

    return(0);
}

