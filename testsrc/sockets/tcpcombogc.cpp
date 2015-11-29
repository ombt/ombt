//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// separate threads for server, clients, accept clients, 
// and a separate thread for garbage clean up, that is,
// joining the threads.

// system headers
#include <string>
#include <list>

// local headers
#include <Debug.h>
#include <logging/Logging.h>
#include <atomic/UseCntPtr.h>
#include <threads/ThreadBarrier.h>
#include <threads/Task.h>
#include <threads/Thread.h>
#include <threads/Queue.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>

using namespace ombt;

// client task
class EchoClient: public Task {
public:
    // ctor and dtor
    EchoClient(const std::string &host, 
               int serverport,
               ThreadBarrier* ptb,
               int msgs_to_send):
        Task(), 
        host_(host), 
        serverport_(serverport), 
        msgs_to_send_(msgs_to_send), 
        ptb_(ptb),
        psocket_(),
        pserver_address_(),
        pconnection_()
    {
        DBGTRACE();
    }
    virtual ~EchoClient()
    {
        DBGTRACE();
    }

    // factory
    virtual Task *clone()
    {
        return NULL;
    }

    // operations
    virtual int init()
    {
        DBGTRACE();
        ptb_->wait();

        DBGPRINTF("%s", "Creating TCP Socket ...");
        psocket_ = new TcpSocket();
        MustBeTrue(psocket_->isOk());

        DBGPRINTF("%s", "Creating IPv4 Server Address ...");
        pserver_address_ = new InetAddress(host_, serverport_);
        MustBeTrue(pserver_address_->isOk());

        DBGPRINTF("%s", "Creating TCP EndPoint ...");
        pconnection_ = new EndPoint(psocket_, NULL);
        MustBeTrue(pconnection_->isOk());

        DBGPRINTF("%s", "Connecting to remote server ...");
        MustBeTrue(pconnection_->connect(*pserver_address_) >= 0);
        MustBeTrue(pconnection_->isOk());

        DBGTRACE();
        return(0);
    }

    virtual int run()
    {
        DBGTRACE();
        char inbuf[BUFSIZ+1];
        char outbuf[BUFSIZ+1];

        size_t nwrite;
        strcpy(outbuf, "hello world !!!");
        nwrite = strlen(outbuf);
        nwrite += 1;

        size_t nread;
        int status;

        DBGTRACE();
        for ( ; msgs_to_send_ > 0; msgs_to_send_--)
        {
            DBGPRINTF("\nMessage left to send ... %d", msgs_to_send_);

            DBGPRINTF("Send message ... %s", outbuf);
            MustBeTrue((status = pconnection_->write(outbuf, nwrite)) >= 0);
            DBGPRINTF("Write status ... %d", status);


            DBGPRINTF("%s", "Waiting for echo message ...");
            nread = BUFSIZ;
            MustBeTrue((status = pconnection_->read(inbuf, nread)) >= 0);
            DBGPRINTF("Read status ... %d", status);

            inbuf[BUFSIZ] = '\0';
            DBGPRINTF("Read ... %s", inbuf);
        }
        return(0);
    }

    virtual int finish()
    {
        DBGTRACE();
        MustBeTrue(pconnection_->close() == 0);
        DBGTRACE();
        return(0);
    }

protected:
    // data
    const std::string host_;
    int serverport_;

    // server/client data
    int msgs_to_send_;
    UseCntPtr<ThreadBarrier> ptb_;
    UseCntPtr<Socket> psocket_;
    UseCntPtr<Address> pserver_address_;
    UseCntPtr<EndPoint> pconnection_;
};

// server task
class EchoClientServer: public Task {
public:
    // ctor and dtor
    EchoClientServer(EndPoint *pConnection, int numberofmessages):
        Task(), 
        numberofmessages_(numberofmessages),
        pconnection_(pConnection)
    {
        DBGTRACE();
    }
    virtual ~EchoClientServer()
    {
        DBGTRACE();
    }

    // factory
    virtual Task *clone()
    {
        return NULL;
    }

    // operations
    virtual int init()
    {
        DBGTRACE();
        return(0);
    }

    virtual int run()
    {
        DBGTRACE();
        int status;
        char buf[BUFSIZ+1];
        size_t count;

        DBGTRACE();
        while (numberofmessages_)
        {
            DBGTRACE();
            count = BUFSIZ;
            if ((status = pconnection_->read(buf, count)) <= 0) break;
            DBGPRINTF("Status from read ... %d", status);
            DBGPRINTF("Read ... %s", buf);

            buf[BUFSIZ] = '\0';
            count = status;

            MustBeTrue((status = pconnection_->write(buf, count)) >= 0);
            DBGPRINTF("Status from write ... %d", status);
            DBGPRINTF("Read ... %s", buf);

            numberofmessages_--;
        }
        DBGTRACE();

        return(0);
    }

    virtual int finish()
    {
        DBGTRACE();
        MustBeTrue(pconnection_->close() == 0);
        DBGTRACE();
        return(0);
    }

protected:
    // client data
    int numberofmessages_;
    UseCntPtr<EndPoint> pconnection_;
};

// server task
class EchoServer: public Task {
public:
    // ctor and dtor
    EchoServer(const std::string &host, 
               int port,
               ThreadBarrier *ptb, 
               int numberofclients,
               int numberofmessages,
               Queue<UseCntPtr<Thread> > *pthreads):
        Task(), 
        host_(host), 
        port_(port), 
        numberofclients_(numberofclients),
        numberofmessages_(numberofmessages),
        pthreads_(pthreads),
        ptb_(ptb), 
        psocket_(),
        paddress_(),
        pconnection_()
    {
        DBGTRACE();
    }
    virtual ~EchoServer()
    {
        DBGTRACE();
    }

    // factory
    virtual Task *clone()
    {
        return NULL;
    }

    // operations
    virtual int init()
    {
        DBGTRACE();

        DBGPRINTF("%s","Creating TCP Socket ...");
        psocket_ = new TcpSocket();
        MustBeTrue(psocket_->isOk());

        DBGPRINTF("%s", "Creating IPv4 Address ...");
        paddress_ = new InetAddress(host_, port_);
        MustBeTrue(paddress_->isOk());

        DBGPRINTF("%s","Creating TCP EndPoint ...");
        pconnection_ = new EndPoint(psocket_, paddress_);
        MustBeTrue(pconnection_->isOk());

        DBGPRINTF("%s","Binding to addresss ...");
        MustBeTrue(pconnection_->bind() == 0);

        DBGPRINTF("%s","Listening to address ...");
        MustBeTrue(pconnection_->listen() == 0);

        DBGTRACE();
        return(0);
    }

    virtual int run()
    {
        DBGTRACE();
        ptb_->unblock();

        char buf[BUFSIZ+1];
        size_t count;

        int clientsleft = numberofclients_;

        while (clientsleft)
        {
            int status;
            UseCntPtr<EndPoint> pclient;

            bool retry = false;
            pclient = pconnection_->accept(retry);
            if (retry) continue;
            MustBeTrue(pclient != NULL);

            UseCntPtr<EchoClientServer> 
                pservertask(new EchoClientServer(pclient, numberofmessages_));
            MustBeTrue(pservertask != NULL);

            UseCntPtr<Thread> pserverthread(new Thread(pservertask));
            MustBeTrue(pserverthread != NULL);
            MustBeTrue(pserverthread->run() == 0);

            pthreads_->enqueue(pserverthread);

            clientsleft--;
        }

        return(0);
    }

    virtual int finish()
    {
        DBGTRACE();
        MustBeTrue(pconnection_->close() == 0);
        DBGTRACE();
        return(0);
    }

protected:
    // data
    const std::string host_;
    int port_;

    // server data
    int numberofclients_;
    int numberofmessages_;
    UseCntPtr<Queue<UseCntPtr<Thread> > > pthreads_;
    UseCntPtr<ThreadBarrier> ptb_;
    UseCntPtr<Socket> psocket_;
    UseCntPtr<Address> paddress_;
    UseCntPtr<EndPoint> pconnection_;
};

// main entry point
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    std::string host("localhost");

    int arg = 0;
    int serverport;
    int numberofclients;
    int numberofmessages;
	
    if (argc == 4)
    {
        serverport = atoi(argv[++arg]);
        numberofclients = atoi(argv[++arg]);
        numberofmessages = atoi(argv[++arg]);
    }
    else
    {
        dbgprintf("usage: %s serverport clientnum msgstosend\n", 
                  argv[arg]);
        return(2);
    }

    dbgprintf("Server port       : %d\n", serverport);
    dbgprintf("Number of clients : %d\n", numberofclients);
    dbgprintf("Number of messages: %d\n", numberofmessages);

    UseCntPtr<ThreadBarrier> ptb(new ThreadBarrier(true));

    UseCntPtr<Queue<UseCntPtr<Thread> > > pthreads;
    pthreads = new Queue<UseCntPtr<Thread> >();

    // total number of threads created
    int totalthreads = 0;
    totalthreads += 1;               // echo server
    totalthreads += numberofclients; // echo client threads
    totalthreads += numberofclients; // accept server threads

    UseCntPtr<EchoServer> 
        pservertask(new EchoServer(host, serverport, ptb, 
                                   numberofclients, numberofmessages,
                                   pthreads));
    MustBeTrue(pservertask != NULL);

    UseCntPtr<Thread> pserverthread(new Thread(pservertask));
    MustBeTrue(pserverthread != NULL);
    MustBeTrue(pserverthread->run() == 0);

    pthreads->enqueue(pserverthread);

    for (int client=1; client<=numberofclients; ++client)
    {
        UseCntPtr<EchoClient> 
            pclienttask(new EchoClient(host, serverport, 
                                       ptb, numberofmessages));
        MustBeTrue(pclienttask != NULL);

        UseCntPtr<Thread> pclientthread(new Thread(pclienttask));
        MustBeTrue(pclientthread != NULL);
        MustBeTrue(pclientthread->run() == 0);

        pthreads->enqueue(pclientthread);
    }

    void *retval;
    UseCntPtr<Thread> pthread;

    while (totalthreads > 0 && pthreads->dequeue(pthread))
    {
        DBGTRACE();
        MustBeTrue(pthread->join(retval) == 0);
        DBGTRACE();
        totalthreads -= 1;
    }
    DBGTRACE();

    return(0);
}

