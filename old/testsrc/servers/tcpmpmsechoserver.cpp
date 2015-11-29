
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/InetAddress.h>
#include <sockets/EndPoint.h>
#include <servers/MultiProcessMultiServiceServer.h>

using namespace ombt;

class MyHandler: public MultiProcMSStreamServer::Handler {
public:
    MyHandler() { ok_ = true; }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint &clientep)
    {
        char buf[BUFSIZ+1];
        size_t count = BUFSIZ;

        count = clientep.read(buf, count);
        if (count > 0)
        {
            if (clientep.write(buf, count) > 0)
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

int
main(int argc, char *argv[])
{
    std::string host("localhost");

    assert(argc == 3);

    int port1 = atoi(argv[1]);
    int port2 = atoi(argv[2]);

    std::cout << "Creating TCP Socket 1 ..." << std::endl;
    TcpSocket *tcp_socket1 = new TcpSocket();
    assert(tcp_socket1->isOk());

    std::cout << "Creating INET Address 1 ..." << std::endl;
    InetAddress *my_address1 = new InetAddress(host, port1);
    assert(my_address1->isOk());

    std::cout << "Creating TcpSocket EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(tcp_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating TcpSocket Socket 2 ..." << std::endl;
    TcpSocket *tcp_socket2 = new TcpSocket();
    assert(tcp_socket2->isOk());

    std::cout << "Creating INET Address 2 ..." << std::endl;
    InetAddress *my_address2 = new InetAddress(host, port2);
    assert(my_address2->isOk());

    std::cout << "Creating TcpSocket EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(tcp_socket2, my_address2));
    assert(server_ep2->isOk());

    MyHandler handler1;
    MyHandler handler2;

    MultiProcMSStreamServer idg;
    assert(idg.isOk());

    idg.registerHandler(*server_ep1, handler1);
    idg.registerHandler(*server_ep2, handler2);

    idg.init();

    idg.run();

    idg.finish();

    return(0);
}

