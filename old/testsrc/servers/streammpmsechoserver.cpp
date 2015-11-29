
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/MultiProcessMultiServiceServer.h>

using namespace ombt;

class MyHandler: 
   public MultiProcMSStreamServer::Handler
{
public:
    MyHandler() { }
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
    std::string filepath1("/tmp/xxxx");
    std::string filepath2("/tmp/yyyy");

    std::cout << "Creating Stream Socket 1 ..." << std::endl;
    StreamSocket *stream_socket1 = new StreamSocket();
    assert(stream_socket1->isOk());

    std::cout << "Creating Local Address 1 ..." << filepath1 << std::endl;
    ::unlink(filepath1.c_str());
    LocalAddress *my_address1 = new LocalAddress(filepath1);
    assert(my_address1->isOk());

    std::cout << "Creating Stream EndPoint 1 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep1(new EndPoint(stream_socket1, my_address1));
    assert(server_ep1->isOk());

    std::cout << "Creating Stream Socket 2 ..." << std::endl;
    StreamSocket *stream_socket2 = new StreamSocket();
    assert(stream_socket2->isOk());

    std::cout << "Creating Local Address 2 ..." << filepath2 << std::endl;
    ::unlink(filepath2.c_str());
    LocalAddress *my_address2 = new LocalAddress(filepath2);
    assert(my_address2->isOk());

    std::cout << "Creating Stream EndPoint 2 ..." << std::endl;
    UseCntPtr<EndPoint> server_ep2(new EndPoint(stream_socket2, my_address2));
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

