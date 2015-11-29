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
class ServerTimerHandler: public IterativeReactor::BaseTimerHandler {
public:
    ServerTimerHandler(): IterativeReactor::BaseTimerHandler() { }
    virtual ~ServerTimerHandler() { }

    virtual int handler(EndPoint *pserver_ep, Timer mytimer) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        dbgprintf("In Timer Handler, sever socket is %d !!!\n", psep->getSocket());
        pserver_->finish();
        return(0);
    }
};

// read handler
class ServerEchoHandler: public IterativeReactor::BaseIOHandler {
public:
    ServerEchoHandler(): IterativeReactor::BaseIOHandler() { }
    virtual ~ServerEchoHandler() { }

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
class ServerAcceptHandler: public IterativeReactor::BaseAcceptHandler {
public:
    ServerAcceptHandler(): IterativeReactor::BaseAcceptHandler() { }
    virtual ~ServerAcceptHandler() { }

    virtual int handler(EndPoint *pserver_ep, EndPoint *paccept_ep) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        UseCntPtr<EndPoint> paep(paccept_ep);
        dbgprintf("In Accept Handler, listen socket is %d, client socket is %d\n", psep->getSocket(), paep->getSocket());
        MustBeTrue(pserver_->registerEndPoint(paep) == OK);
        UseCntPtr<IterativeReactor::BaseIOHandler> pbioh(new ServerEchoHandler);
        MustBeTrue(pserver_->registerReadHandler(paep, pbioh) == OK);
        return(0);
    }
};

// signal handler
class ServerSignalHandler: public IterativeReactor::BaseSignalHandler {
public:
    ServerSignalHandler(): IterativeReactor::BaseSignalHandler() { }
    virtual ~ServerSignalHandler() { }

    virtual int handler(EndPoint *pserver_ep, int signalno) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        dbgprintf("In Signal Handler, signal socket is %d, signal is %d\n", psep->getSocket(), signalno);
        //pserver_->finish();
        //SignalEndPoint *prawsep = dynamic_cast<SignalEndPoint *>(pserver_ep);
        //if (prawsep) prawsep->unblock();
        exit(2);
        return(1);
    }
};

// server thread task
class ServerTask: public Task {
public:
    ServerTask(int seconds, int port, EndPoint* psep,
               IterativeReactor::BaseSignalHandler *pbsh,
               ThreadBarrier *ptb):
        Task(), seconds_(seconds), port_(port), 
        psep_(psep), pbsh_(pbsh), ptb_(ptb) { }
    ~ServerTask() { }

    virtual int init() {
        dbgprintf("Server: Create an IterativeReactor ...\n");
        preactor_ = UseCntPtr<IterativeReactor>(new IterativeReactor(IterativeReactor::EPoll));
        MustBeTrue(preactor_ != NULL);

        dbgprintf("Server: Create a TCP socket ...\n");
        ptcpsocket_ = UseCntPtr<Socket>(new TcpSocket());
        MustBeTrue(ptcpsocket_->isOk());

        dbgprintf("Server: Create an INET address ...\n");
        paddress_ = UseCntPtr<InetAddress>(new InetAddress("localhost", port_));
        MustBeTrue(paddress_->isOk());

        dbgprintf("Server: Create an EndPoint ...\n");
        pendpoint_ = UseCntPtr<EndPoint>(new EndPoint(ptcpsocket_, paddress_));
        MustBeTrue(pendpoint_->isOk());

        dbgprintf("Server: Bind EndPoint ...\n");
        MustBeTrue(pendpoint_->bind() == OK);

        dbgprintf("Server: Listen on EndPoint ...\n");
        MustBeTrue(pendpoint_->listen() == OK);

        dbgprintf("Server: Create Accept Handler ...\n");
        UseCntPtr<IterativeReactor::BaseAcceptHandler> pbah(new ServerAcceptHandler);

        dbgprintf("Server: Register EndPoint ...\n");
        MustBeTrue(preactor_->registerEndPoint(pendpoint_) == OK);

        dbgprintf("Server: Register accept handler ...\n");
        MustBeTrue(preactor_->registerAcceptHandler(pendpoint_, pbah) == OK);

        dbgprintf("Server: Create a %d second timer...\n", seconds_);
        Timer tmout(seconds_, 0);

        dbgprintf("Server: Create Timer Handler ...\n");
        UseCntPtr<IterativeReactor::BaseTimerHandler> pbth(new ServerTimerHandler);

        dbgprintf("Server: Schedule a timer ...\n");
        MustBeTrue(preactor_->scheduleTimer(pendpoint_, tmout, pbth) == OK);

        dbgprintf("Server: Register Signal EndPoint ...\n");
        MustBeTrue(preactor_->registerEndPoint(psep_) == OK);

        dbgprintf("Server: Register Signal handler ...\n");
        MustBeTrue(preactor_->registerSignalHandler(psep_, pbsh_) == OK);

        dbgprintf("Server: Run init() ...\n");
        MustBeTrue(preactor_->init() == OK);

        return 0;
    }
    virtual int run() {
        dbgprintf("Server: Run run() and unblock() ...\n");
        ptb_->unblock();
        dbgprintf("Server: n run() and after unblock() ...\n");
        MustBeTrue(preactor_->run() == OK);
        return 0;
    }
    virtual int finish() {
        dbgprintf("Server: Run finish() ...\n");
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
    UseCntPtr<ThreadBarrier> ptb_;
};

// client read handler
class ClientReadHandler: public IterativeReactor::BaseIOHandler {
public:
    ClientReadHandler(): IterativeReactor::BaseIOHandler() { }
    virtual ~ClientReadHandler() { }

    virtual int handler(EndPoint *pserver_ep) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        dbgprintf("Client Read Handler, socket is %d\n", psep->getSocket());

        char buf[1024];
        size_t count = 1024;
        int status = psep->read(buf, count);
        if (status > 0)
        {
            dbgprintf("Client Read Handler, echo'ed buffer is <%s>\n", buf);
            return(1);
        }
        else if (status == 0)
            return(1);
        else if (status < 0)
            return(-1);
    }
};

// client write handler
class ClientWriteHandler: public IterativeReactor::BaseIOHandler {
public:
    ClientWriteHandler(): IterativeReactor::BaseIOHandler() {
    }
    virtual ~ClientWriteHandler() {
    }

    virtual int handler(EndPoint *pserver_ep) {
        UseCntPtr<EndPoint> psep(pserver_ep);
        dbgprintf("Client Write Handler, socket is %d\n", psep->getSocket());

        char buf[1024];
        strcpy(buf, "hello world !!!");
        size_t nwrite = strlen(buf) + 1;

        int status = psep->write(buf, nwrite);
        if (status == 0)
            return(1);
        else if (status < 0)
            return(-1);
        else
            return(0);
    }
};

// client thread task
class ClientTask: public Task {
public:
    ClientTask(int port, ThreadBarrier *ptb): 
        Task(), port_(port), ptb_(ptb) { }
    ~ClientTask() { }

    virtual int init() {
        dbgprintf("Client: init() waiting on thread barrier ...\n");
        ptb_->wait();
        dbgprintf("Client: init() released from thread barrier ...\n");

        dbgprintf("Client: Creating TCP Socket ...\n");
        psocket_ = new TcpSocket();
        MustBeTrue(psocket_->isOk());

        dbgprintf("Client: Creating IPv4 Server Address ...\n");
        paddress_ = new InetAddress("localhost", port_);
        MustBeTrue(paddress_->isOk());

        dbgprintf("Creating TCP EndPoint ...\n");
        pendpoint_ = new EndPoint(psocket_, NULL);
        MustBeTrue(pendpoint_->isOk());

        dbgprintf("Connecting to remote server ...\n");
        MustBeTrue(pendpoint_->connect(*paddress_) >= 0);
        MustBeTrue(pendpoint_->isOk());

        dbgprintf("Client: Create an IterativeReactor ...\n");
        preactor_ = UseCntPtr<IterativeReactor>(new IterativeReactor(IterativeReactor::EPoll));
        MustBeTrue(preactor_ != NULL);

        dbgprintf("Client: Create Read Handler ...\n");
        UseCntPtr<IterativeReactor::BaseIOHandler> pbrh(new ClientReadHandler);

        dbgprintf("Client: Create write Handler ...\n");
        UseCntPtr<IterativeReactor::BaseIOHandler> pbwh(new ClientWriteHandler);

        dbgprintf("Client: Register Client EndPoint ...\n");
        MustBeTrue(preactor_->registerEndPoint(pendpoint_) == OK);

        dbgprintf("Client: Register read handler ...\n");
        MustBeTrue(preactor_->registerReadHandler(pendpoint_, pbrh) == OK);

        dbgprintf("Client: Register write handler ...\n");
        MustBeTrue(preactor_->registerWriteHandler(pendpoint_, pbwh) == OK);

        dbgprintf("Client: Run init() ...\n");
        MustBeTrue(preactor_->init() == OK);

        char buf[1024];
        strcpy(buf, "hello world !!!");
        size_t nwrite = strlen(buf) + 1;

        dbgprintf("Client: send msg to server ...\n");
        int status = pendpoint_->write(buf, nwrite);
        if (status == 0)
            return(1);
        else if (status < 0)
            return(-1);
        else
            return(0);
    }
    virtual int run() {
        dbgprintf("Client: Run run() ...\n");
        MustBeTrue(preactor_->run() == OK);
        return 0;
    }
    virtual int finish() {
        dbgprintf("Client: Run finish() ...\n");
        MustBeTrue(preactor_->finish() == OK);
        return 0;
    }

private:
    int port_;
    UseCntPtr<IterativeReactor> preactor_;
    UseCntPtr<Socket> psocket_;
    UseCntPtr<InetAddress> paddress_;
    UseCntPtr<EndPoint> pendpoint_;
    UseCntPtr<ThreadBarrier> ptb_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " seconds port msgs" << endl;
        return(2);
    }

    int arg = 0;
    int seconds = atoi(argv[++arg]);
    int port = atoi(argv[++arg]);
    int nmsgs = atoi(argv[++arg]);

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
    UseCntPtr<IterativeReactor::BaseSignalHandler> pbsh(new ServerSignalHandler);

    UseCntPtr<ThreadBarrier> ptb(new ThreadBarrier(true));

    UseCntPtr<Thread> pserver(new Thread(new ServerTask(seconds, port, psep, pbsh, ptb)));
    MustBeTrue(pserver->isOk());
    MustBeTrue(pserver->run() == 0);
    MustBeTrue(pserver->isOk());

    void *retval;
    for (int i=1; i<=nmsgs; ++i)
    {
        dbgprintf("\nMSG number %d ...\n", i);
        UseCntPtr<Thread> pclient(new Thread(new ClientTask(port, ptb)));
        MustBeTrue(pclient->isOk());
        MustBeTrue(pclient->run() == 0);
        MustBeTrue(pclient->isOk());
        pclient->join(retval);
    }

    pserver->join(retval);

    return(0);
}

