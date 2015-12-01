//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// combined multiple stream clients and one stream server threads

// system headers
#include <string>
#include <list>
#include <sstream>

// local headers
#include "system/Debug.h"
#include "logging/Logging.h"
#include "atomic/UseCntPtr.h"
#include "threads/ThreadBarrier.h"
#include "threads/Task.h"
#include "threads/Thread.h"
#include "sockets/Socket.h"
#include "sockets/LocalAddress.h"
#include "sockets/EndPoint.h"

using namespace ombt;

// client task
class EchoClient: public Task {
public:
    // ctor and dtor
    EchoClient(const std::string &host, 
               const std::string &serverpath,
               ThreadBarrier* ptb,
               int msgs_to_send):
        Task(), 
        host_(host), 
        serverpath_(serverpath), 
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

        DBGPRINTF("%s", "Creating Stream Socket ...");
        psocket_ = new StreamSocket();
        MustBeTrue(psocket_->isOk());

        DBGPRINTF("%s", "Creating Local Server Address ...");
        pserver_address_ = new LocalAddress(serverpath_);
        MustBeTrue(pserver_address_->isOk());

        DBGPRINTF("%s", "Creating Stream EndPoint ...");
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
    const std::string serverpath_;

    // server/client data
    int msgs_to_send_;
    UseCntPtr<ThreadBarrier> ptb_;
    UseCntPtr<Socket> psocket_;
    UseCntPtr<Address> pserver_address_;
    UseCntPtr<EndPoint> pconnection_;
};

// server task
class EchoServer: public Task {
public:
    // ctor and dtor
    EchoServer(const std::string &host, 
               const std::string &serverpath,
               ThreadBarrier *ptb, 
               int msgs_to_read):
        Task(), 
        host_(host), 
        serverpath_(serverpath),
        msgs_to_read_(msgs_to_read),
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

        DBGPRINTF("%s","Creating Stream Socket ...");
        psocket_ = new StreamSocket();
        MustBeTrue(psocket_->isOk());

        DBGPRINTF("%s", "Creating Local Address ...");
        paddress_ = new LocalAddress(serverpath_);
        MustBeTrue(paddress_->isOk());

        DBGPRINTF("%s","Creating Stream EndPoint ...");
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

        while (msgs_to_read_)
        {
            int status;
            UseCntPtr<EndPoint> pclient;

            DBGPRINTF("%s","Waiting for a message from a new client...");

            bool retry = false;
            pclient = pconnection_->accept(retry);
            if (retry) continue;
            MustBeTrue(pclient != NULL);

            while (true)
            {
                count = BUFSIZ;
                if ((status = pclient->read(buf, count)) <= 0) break;
                DBGPRINTF("Status from read ... %d", status);
                DBGPRINTF("Read ... %s", buf);

                buf[BUFSIZ] = '\0';
                count = status;

                MustBeTrue((status = pclient->write(buf, count)) >= 0);
                DBGPRINTF("Status from write ... %d", status);
                DBGPRINTF("Read ... %s", buf);

                msgs_to_read_--;
            }

            DBGPRINTF("%s", "Closing client socket ...");
            MustBeTrue(pclient->close() == 0);
            DBGTRACE();
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
    const std::string serverpath_;

    // server data
    int msgs_to_read_;
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
    std::string serverpath("");

    int arg = 0;
    int numberofclients;
    int numberofmessages;
	
    if (argc == 4)
    {
        serverpath = argv[++arg];
        numberofclients = atoi(argv[++arg]);
        numberofmessages = atoi(argv[++arg]);
    }
    else
    {
        dbgprintf("usage: %s serverpath clientnum msgstosend\n", 
                  argv[arg]);
        return(2);
    }

    ::unlink(serverpath.c_str());

    dbgprintf("Server path       : %s\n", serverpath.c_str());
    dbgprintf("Number of clients : %d\n", numberofclients);
    dbgprintf("Number of messages: %d\n", numberofmessages);

    UseCntPtr<ThreadBarrier> ptb(new ThreadBarrier(true));


    UseCntPtr<EchoServer> 
        pservertask(new EchoServer(host, serverpath, ptb, 
                                   numberofclients*numberofmessages));
    MustBeTrue(pservertask != NULL);

    UseCntPtr<Thread> pserverthread(new Thread(pservertask));
    MustBeTrue(pserverthread != NULL);
    MustBeTrue(pserverthread->run() == 0);

    std::list<UseCntPtr<Thread> > threads;

    for (int client=1; client<=numberofclients; ++client)
    {
        UseCntPtr<EchoClient> 
            pclienttask(new EchoClient(host, serverpath,
                                       ptb, numberofmessages));
        MustBeTrue(pclienttask != NULL);

        UseCntPtr<Thread> pclientthread(new Thread(pclienttask));
        MustBeTrue(pclientthread != NULL);
        MustBeTrue(pclientthread->run() == 0);

        threads.push_back(pclientthread);
    }

    void *retval;

    std::list<UseCntPtr<Thread> >::iterator cit = threads.begin();
    std::list<UseCntPtr<Thread> >::iterator citend = threads.end();
    DBGTRACE();
    for ( ; cit != citend; ++cit)
    {
        DBGTRACE();
        MustBeTrue((*cit)->join(retval) == 0);
        DBGTRACE();
    }
    DBGTRACE();
    MustBeTrue(pserverthread->join(retval) == 0);
    DBGTRACE();

    return(0);
}

