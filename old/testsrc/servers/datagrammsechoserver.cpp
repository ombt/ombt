
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/IterativeMultiServiceServer.h>

using namespace ombt;

class MyHandler: public IterativeMSDatagramServer::Handler {
public:
    MyHandler() { ok_ = true; }
    virtual ~MyHandler() { }

    virtual int operator()(EndPoint &serverep)
    {
        count = BUFSIZ;

        UseCntPtr<Address> peer(serverep.getInternalAddress().create());

        count = serverep.read(buf, count, *peer);
        if (count > 0)
        {
            if (serverep.write(buf, count, *peer) > 0)
                return(0);
            else
                return(-1);
        }
        else if (count < 0)
            return(-1); // error
        else
            return(1); // EOF -- all done
    }

    size_t count;
    char buf[BUFSIZ+1];
};

int
main(int argc, char *argv[])
{
    std::string filepath1("/tmp/xxxyyy");
    std::string filepath2("/tmp/yyyzzz");

    std::cout << "Creating Datagram Socket 1 ..." << std::endl;
    DatagramSocket *datagram_socket1 = new DatagramSocket();
    assert(datagram_socket1->isOk());

    std::cout << "Creating Local Address 1 ... " << filepath1 << std::endl;
    ::unlink(filepath1.c_str());
    LocalAddress *my_address1 = new LocalAddress(filepath1);
    assert(my_address1->isOk());

    std::cout << "Creating Datagram EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(datagram_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating Datagram Socket 2 ..." << std::endl;
    DatagramSocket *datagram_socket2 = new DatagramSocket();
    assert(datagram_socket2->isOk());

    std::cout << "Creating Local Address 2 ... " << filepath2 << std::endl;
    ::unlink(filepath2.c_str());
    LocalAddress *my_address2 = new LocalAddress(filepath2);
    assert(my_address2->isOk());

    std::cout << "Creating Datagram EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(datagram_socket2, my_address2));
    assert(server_ep2->isOk());

    MyHandler handler1;
    MyHandler handler2;

    IterativeMSDatagramServer idg;
    assert(idg.isOk());

    idg.registerHandler(*server_ep1, handler1);
    idg.registerHandler(*server_ep2, handler2);

    assert(idg.init() == 0);

    assert(idg.run() == 0);;

    assert(idg.finish() == 0);

    return(0);
}

