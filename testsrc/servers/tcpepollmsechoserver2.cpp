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
#include <sockets/InetAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeMultiServiceServer.h>

using namespace ombt;

class MyHandler: public IterativeMSStreamServer::Handler
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
    std::string host("localhost");

    if (argc != 5)
    {
        std::cerr << "usage: " << argv[0] << " port1 port2 time1 time2" << std::endl;
        return(0);
    }

    int port1 = atoi(argv[1]);
    int port2 = atoi(argv[2]);
    int timer1 = atoi(argv[3]);
    int timer2 = atoi(argv[4]);

    std::cout << "Creating TCP Socket 1 ..." << std::endl;
    UseCntPtr<TcpSocket> tcp_socket1(new TcpSocket());
    assert(tcp_socket1->isOk());

    std::cout << "Creating INET Address 1 ... " << port1 << std::endl;
    UseCntPtr<InetAddress> my_address1(new InetAddress(host, port1));
    assert(my_address1->isOk());

    std::cout << "Creating TcpSocket EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(tcp_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating TcpSocket Socket 2 ..." << std::endl;
    UseCntPtr<TcpSocket> tcp_socket2(new TcpSocket());
    assert(tcp_socket2->isOk());

    std::cout << "Creating INET Address 2 ... "  << port2 << std::endl;
    UseCntPtr<InetAddress> my_address2(new InetAddress(host, port2));
    assert(my_address2->isOk());

    std::cout << "Creating TcpSocket EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(tcp_socket2, my_address2));
    assert(server_ep2->isOk());

    UseCntPtr<MyHandler> handler1(new MyHandler);
    UseCntPtr<MyHandler> handler2(new MyHandler);

    IterativeMSStreamServer idg(Server::EPoll);
    assert(idg.isOk());

    idg.registerHandler(server_ep1, handler1);
    idg.registerHandler(server_ep2, handler2);

    UseCntPtr<MyTimerHandler> thandler1(new MyTimerHandler);
    UseCntPtr<MyTimerHandler> thandler2(new MyTimerHandler);

    idg.registerTimerHandler(server_ep1, thandler1);
    idg.registerTimerHandler(server_ep2, thandler2);

    std::cout << "Timer 1 ... " << timer1 << std::endl;
    Timer t1(timer1,0);

    std::cout << "Timer 2 ... " << timer2 << std::endl;
    Timer t2(timer2,0);

    idg.scheduleTimer(server_ep1, t1);
    idg.scheduleTimer(server_ep2, t2);

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

