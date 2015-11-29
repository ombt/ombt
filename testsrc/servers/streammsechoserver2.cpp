//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeMultiServiceServer.h>

using namespace ombt;

class MyHandler:
   public IterativeMSStreamServer::Handler
{
public:
    MyHandler() { }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint *clientep)
    {
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

        UseCntPtr<EndPoint> pclientep(clientep);
        count = pclientep->read(buf, count);
        if (count > 0)
        {
            if (pclientep->write(buf, count) > 0)
                return(0);
            else
            {
                DUMP(errno);
                return(-1);
            }
        }
        else if (count == 0)
            return(1);
        else
        {
            DUMP(errno);
            return(-1);
        }
    }
};

class MyTimerHandler: public IterativeMSStreamServer::TimerHandler
{
public:
    MyTimerHandler() { }
    virtual ~MyTimerHandler() { }

    virtual int operator()(EndPoint *clientep, Timer &timer)
    {
        TRACE();
        return(1);
    }
};

int
main(int argc, char *argv[])
{
    int t1secs = 10;
    int t2secs = 20;

    if (argc == 2)
    {
        t1secs = atoi(argv[1]);;
    }
    else if (argc == 3)
    {
        t1secs = atoi(argv[1]);;
        t2secs = atoi(argv[2]);;
    }

    std::string filepath1("/tmp/xxxx");
    std::string filepath2("/tmp/yyyy");

    std::cout << "Creating Stream Socket 1 ..." << std::endl;
    UseCntPtr<StreamSocket> stream_socket1(new StreamSocket());
    assert(stream_socket1->isOk());

    std::cout << "Creating Local Address 1 ..." << filepath1 << std::endl;
    ::unlink(filepath1.c_str());
    UseCntPtr<LocalAddress> my_address1(new LocalAddress(filepath1));
    assert(my_address1->isOk());

    std::cout << "Creating Stream EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(stream_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating Stream Socket 2 ..." << std::endl;
    UseCntPtr<StreamSocket> stream_socket2(new StreamSocket());
    assert(stream_socket2->isOk());

    std::cout << "Creating Local Address 2 ..." << filepath2 << std::endl;
    ::unlink(filepath2.c_str());
    UseCntPtr<LocalAddress> my_address2(new LocalAddress(filepath2));
    assert(my_address2->isOk());

    std::cout << "Creating Stream EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(stream_socket2, my_address2));
    assert(server_ep2->isOk());

    UseCntPtr<MyHandler> handler1(new MyHandler);
    UseCntPtr<MyHandler> handler2(new MyHandler);

    IterativeMSStreamServer idg;
    assert(idg.isOk());

    idg.registerHandler(server_ep1, handler1);
    idg.registerHandler(server_ep2, handler2);

    UseCntPtr<MyTimerHandler> thandler1(new MyTimerHandler);
    UseCntPtr<MyTimerHandler> thandler2(new MyTimerHandler);

    idg.registerTimerHandler(server_ep1, thandler1);
    idg.registerTimerHandler(server_ep2, thandler2);

    std::cout << "T1 secs ..." << t1secs << std::endl;
    Timer t1(t1secs,0);
    std::cout << "T2 secs ..." << t2secs << std::endl;
    Timer t2(t2secs,0);

    idg.scheduleTimer(server_ep1, t1);
    idg.scheduleTimer(server_ep2, t2);

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

