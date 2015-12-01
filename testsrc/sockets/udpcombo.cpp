//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// combined client and server threads

// system headers
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>

// local headers
#include "system/Debug.h"
#include "atomic/UseCntPtr.h"
#include "threads/Task.h"
#include "threads/Thread.h"
#include "logging/Logging.h"
#include "sockets/Socket.h"
#include "sockets/InetAddress.h"
#include "sockets/EndPoint.h"
#include "threads/ThreadBarrier.h"

using namespace ombt;

// number of messages to send
static int numberOfMsgsToSend = 10;

// client task
class EchoClient: public Task {
public:
    // ctor and dtor
    EchoClient(const std::string &host, int serverport, ThreadBarrier* ptb,
               int msgs_to_send):
        Task(), host_(host), serverport_(serverport),
        msgs_to_send_(msgs_to_send), ptb_(ptb)
    {
        DBGTRACE();
    }
    virtual ~EchoClient()
    {
        DBGTRACE();
    }

    // factory
    virtual Task *clone() { return NULL; }

    // operations
    virtual int init()
    {
        DBGTRACE();
        ptb_->wait();

        DBGTRACE();
        psocket_= new UdpSocket();
        MustBeTrue(psocket_->isOk());

        DBGTRACE();
        pserver_address_ = new InetAddress(host_, serverport_);
        MustBeTrue(pserver_address_->isOk());

        DBGTRACE();
        pconnection_ = new EndPoint(psocket_, NULL);
        MustBeTrue(pconnection_->isOk());

        DBGTRACE();
        return(0);
    }
    virtual int run()
    {
        DBGTRACE();

        char inbuf[BUFSIZ+1];
        char outbuf[BUFSIZ+1];
        size_t count;
        int status;

        for ( ; msgs_to_send_ > 0; msgs_to_send_--)
        {
            strcpy(outbuf, "hello world !!!");
            count = strlen(outbuf);
            count += 1;

            DBGPRINTF("Client: messages left to send ... %d", msgs_to_send_);
            DBGPRINTF("Client: Sending msg ... %s", outbuf);
            MustBeTrue((status = pconnection_->write(outbuf, count, *pserver_address_)) >= 0);
            DBGPRINTF("Client: Write status ... %d",  status);

            DBGPRINTF("%s", "Client: Waiting for echo message ...");
            InetAddress peer_address;
            MustBeTrue((status = pconnection_->read(inbuf, count, peer_address)) >= 0);
            DBGPRINTF("Client: read status ... %d",  status);

            inbuf[BUFSIZ] = '\0';
            DBGPRINTF("Client: Read msg ... %s", inbuf);
        }

        return(0);
    }
    virtual int finish()
    {
        DBGTRACE();
        MustBeTrue(pconnection_->close() == 0);
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
class EchoServer: public Task {
public:
    // ctor and dtor
    EchoServer(const std::string &host, int port,
               ThreadBarrier *ptb, int msgs_to_read):
        Task(), host_(host), port_(port), ptb_(ptb), msgs_to_read_(msgs_to_read)
    {
        DBGTRACE();
    }
    virtual ~EchoServer()
    {
        DBGTRACE();
    }

    // factory
    virtual Task *clone() { return NULL; }

    // operations
    virtual int init()
    {
        DBGTRACE();
        psocket_ = new UdpSocket();
        MustBeTrue(psocket_->isOk());

        DBGTRACE();
        paddress_ = new InetAddress(host_, port_);
        MustBeTrue(paddress_->isOk());

        DBGTRACE();
        pconnection_ = new EndPoint(psocket_, paddress_);
        MustBeTrue(pconnection_->isOk());

        DBGTRACE();
        MustBeTrue(pconnection_->bind() == 0);

        DBGTRACE();
        return(0);
    }
    virtual int run()
    {
        DBGTRACE();

        int status;
        InetAddress peer_address;
        char buf[BUFSIZ+1];
        size_t count;

        DBGTRACE();
        ptb_->unblock();

        DBGTRACE();
        for ( ; msgs_to_read_ > 0; msgs_to_read_--)
        {
            int status;

            count = BUFSIZ;
            DBGPRINTF("Server: Waiting for a message ... %d", msgs_to_read_);
            MustBeTrue((status = pconnection_->read(buf, count, peer_address)) >= 0);
            DBGPRINTF("Server: Read status ... %d", status);

            buf[BUFSIZ] = '\0';
            count = status;

            DBGPRINTF("Server: Read ... %s", buf);
            MustBeTrue((status = pconnection_->write(buf, count, peer_address)) >= 0);
            DBGPRINTF("Server: Write status ... %d", status);
            DBGPRINTF("Server: Wrote ... %s", buf);
        }

        return(0);
    }
    virtual int finish()
    {
        DBGTRACE();
        MustBeTrue(pconnection_->close() == 0);
        return(0);
    }

protected:
    // data
    const std::string host_;
    int port_;

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

    int arg = 0;
    if (argc == 2)
    {
        numberOfMsgsToSend = atoi(argv[++arg]);
    }
    else if (argc > 2)
    {
        DBGPRINTF("usage: %s [number_msgs]", argv[0]);
        return(2);
    }

    std::stringstream ss;
    ss << ::getpid();

    std::string host("localhost");

    int serverport = 55555;

    DBGPRINTF("Server Host: %s",  host.c_str());
    DBGPRINTF("Server Port: %d",  serverport);

    UseCntPtr<ThreadBarrier> ptb(new ThreadBarrier(true));

    UseCntPtr<EchoServer> pservertask(new EchoServer(host, serverport,
                                      ptb, numberOfMsgsToSend));
    MustBeTrue(pservertask != NULL);

    UseCntPtr<Thread> pserverthread(new Thread(pservertask));
    MustBeTrue(pserverthread != NULL);

    UseCntPtr<EchoClient> pclienttask(new EchoClient(host, serverport, 
                                      ptb, numberOfMsgsToSend));
    MustBeTrue(pclienttask != NULL);

    UseCntPtr<Thread> pclientthread(new Thread(pclienttask));
    MustBeTrue(pclientthread != NULL);

    MustBeTrue(pserverthread->run() == 0);
    
    MustBeTrue(pclientthread->run() == 0);

    void *retval;
    MustBeTrue(pserverthread->join(retval) == 0);
    MustBeTrue(pclientthread->join(retval) == 0);

    return(0);
}

