
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <atomic/UseCntPtr.h>
#include <sockets/Socket.h>
#include <sockets/LocalAddress.h>
#include <sockets/EndPoint.h>
#include <servers/MultiThreadServer.h>

using namespace ombt;

class MyHandler: public MultiThreadStreamServer::Handler {
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
                return(-1);
        }
        else if (count == 0)
            return(1);
        else
            return(-1);
    }
};

int
main(int argc, char *argv[])
{
    std::string host("localhost");
    std::string filepath("/tmp/XXX");

    if (argc == 2)
    {
        filepath = argv[1];
    }
    else
    {
        std::cout << "usage: " << argv[0] << " file_path" << std::endl;
        return(2);
    }

    std::cout << "Creating Streams Socket ..." << std::endl;
    StreamSocket *stream_socket = new StreamSocket();
    assert(stream_socket->isOk());

    std::cout << "Creating Local Address ..." << std::endl;
    ::unlink(filepath.c_str());
    LocalAddress *my_address = new LocalAddress(filepath);
    assert(my_address->isOk());

    std::cout << "Creating Stream EndPoint ..." << std::endl;
    UseCntPtr<EndPoint> server_ep(new EndPoint(stream_socket, my_address));
    assert(server_ep->isOk());

    MyHandler iss_handler;
    MultiThreadStreamServer iss(*server_ep, iss_handler);

    iss.init();

    iss.run();

    iss.finish();

    return(0);
}

