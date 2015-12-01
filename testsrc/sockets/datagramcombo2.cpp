//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// combined multiple clients and one server threads

// system headers
#include <string>
#include <sstream>
#include <list>

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
               const std::string &path,
               const std::string &serverpath, 
               ThreadBarrier* ptb,
               int msgs_to_send):
        Task(), 
        host_(host), 
        path_(path), 
        serverpath_(serverpath),
        msgs_to_send_(msgs_to_send), 
        ptb_(ptb),
        psocket_(),
        pserver_address_(),
        pclient_address_(),
        pconnection_()
    {
        DBGPRINTF("%s", "EchoClient ctor");
    }
    virtual ~EchoClient()
    {
        DBGPRINTF("%s", "EchoClient dtor");
    }

    // factory
    virtual Task *clone()
    {
        return NULL;
    }

    // operations
    virtual int init()
    {
        ptb_->wait();

        psocket_= new DatagramSocket();
        MustBeTrue(psocket_->isOk());

        pserver_address_ = new LocalAddress(serverpath_);
        MustBeTrue(pserver_address_->isOk());

        ::unlink(path_.c_str());

        pclient_address_ = new LocalAddress(path_);
        MustBeTrue(pclient_address_->isOk());

        pconnection_ = new EndPoint(psocket_, pclient_address_);
        MustBeTrue(pconnection_->isOk());

        MustBeTrue(pconnection_->bind() == 0);

        return(0);
    }

    virtual int run()
    {
        char inbuf[BUFSIZ+1];
        char outbuf[BUFSIZ+1];
        size_t count;
        int status;

        strcpy(outbuf, "hello world !!!");

        for ( ; msgs_to_send_ > 0; msgs_to_send_--)
        {
            count = strlen(outbuf);
            count += 1;

            MustBeTrue((status = pconnection_->write(outbuf, count, *pserver_address_)) >= 0);

            LocalAddress peer_address;
            MustBeTrue((status = pconnection_->read(inbuf, count, peer_address)) >= 0);

            inbuf[BUFSIZ] = '\0';
        }

        return(0);
    }

    virtual int finish()
    {
        MustBeTrue(pconnection_->close() == 0);
        return(0);
    }

protected:
    // data
    const std::string host_;
    const std::string path_;
    const std::string serverpath_;

    // server/client data
    int msgs_to_send_;
    UseCntPtr<ThreadBarrier> ptb_;
    UseCntPtr<Socket> psocket_;
    UseCntPtr<Address> pserver_address_;
    UseCntPtr<Address> pclient_address_;
    UseCntPtr<EndPoint> pconnection_;
};

// server task
class EchoServer: public Task {
public:
    // ctor and dtor
    EchoServer(const std::string &host, 
               const std::string &path, 
               ThreadBarrier *ptb, 
               int msgs_to_read):
        Task(), 
        host_(host), 
        path_(path), 
        msgs_to_read_(msgs_to_read),
        ptb_(ptb), 
        psocket_(),
        paddress_(),
        pconnection_()
    {
        DBGPRINTF("%s", "EchoServer ctor");
    }
    virtual ~EchoServer()
    {
        DBGPRINTF("%s", "EchoServer dtor");
    }

    // factory
    virtual Task *clone()
    {
        return NULL;
    }

    // operations
    virtual int init()
    {
        psocket_ = new DatagramSocket();
        MustBeTrue(psocket_->isOk());

        ::unlink(path_.c_str());

        paddress_ = new LocalAddress(path_);
        MustBeTrue(paddress_->isOk());

        pconnection_ = new EndPoint(psocket_, paddress_);
        MustBeTrue(pconnection_->isOk());

        MustBeTrue(pconnection_->bind() == 0);

        return(0);
    }

    virtual int run()
    {
        int status;
        LocalAddress peer_address;
        char buf[BUFSIZ+1];
        size_t count;

        ptb_->unblock();

        for (int msg=1 ; msg<=msgs_to_read_; ++msg)
        {
            int status;

            count = BUFSIZ;
            MustBeTrue((status = pconnection_->read(buf, count, peer_address)) >= 0);

            buf[BUFSIZ] = '\0';
            count = status;

            MustBeTrue((status = pconnection_->write(buf, count, peer_address)) >= 0);

            DBGPRINTF("EchoServer: echoed msg %d", msg);
        }

        return(0);
    }
    virtual int finish()
    {
        MustBeTrue(pconnection_->close() == 0);
        return(0);
    }

protected:
    // data
    const std::string host_;
    const std::string path_;

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
    int numberofclients;
    int numberofmessages;
	
    if (argc == 3)
    {
        numberofclients = atoi(argv[++arg]);
        numberofmessages = atoi(argv[++arg]);
    }
    else
    {
        dbgprintf("usage: %s number_of_clients number_of_messages\n", 
                  argv[arg]);
        return(2);
    }

    dbgprintf("Number of clients : %d\n", numberofclients);
    dbgprintf("Number of messages: %d\n", numberofmessages);

    UseCntPtr<ThreadBarrier> ptb(new ThreadBarrier(true));

    std::stringstream ss;
    ss << ::getpid();

    std::string host("localhost");
    std::string serverpath("/tmp/server" + ss.str());
    std::string clientprefix("/tmp/client" + ss.str());

    dbgprintf("Host       : %s\n",  host.c_str());
    dbgprintf("Server Path: %s\n",  serverpath.c_str());

    UseCntPtr<EchoServer> pservertask(new EchoServer(host, serverpath, ptb, numberofclients*numberofmessages));
    MustBeTrue(pservertask != NULL);

    UseCntPtr<Thread> pserverthread(new Thread(pservertask));
    MustBeTrue(pserverthread != NULL);
    MustBeTrue(pserverthread->run() == 0);

    std::list<UseCntPtr<Thread> > threads;

    for (int client=1; client<=numberofclients; ++client)
    {
        std::stringstream clientnumber;
        clientnumber << client;
        std::string clientpath = clientprefix + clientnumber.str();
        dbgprintf("Client Path: %s\n",  clientpath.c_str());

        UseCntPtr<EchoClient> pclienttask(new EchoClient(host, clientpath, 
                                          serverpath, ptb, numberofmessages));
        MustBeTrue(pclienttask != NULL);

        UseCntPtr<Thread> pclientthread(new Thread(pclienttask));
        MustBeTrue(pclientthread != NULL);
        MustBeTrue(pclientthread->run() == 0);

        threads.push_back(pclientthread);
    }

    void *retval;
    MustBeTrue(pserverthread->join(retval) == 0);

    std::list<UseCntPtr<Thread> >::iterator cit = threads.begin();
    std::list<UseCntPtr<Thread> >::iterator citend = threads.begin();
    for ( ; cit != citend; ++cit)
    {
        MustBeTrue((*cit)->join(retval) == 0);
    }

    return(0);
}
