#ifndef __OMBT_MULTI_PROCESS_SERVER_H
#define __OMBT_MULTI_PROCESS_SERVER_H

// multiprocess single-service server

// headers
#include <stdio.h>
#include <servers/Server.h>
#include <sockets/Address.h>
#include <sockets/EndPoint.h>

namespace ombt {

// forking server for a connection
class MultiProcessDatagramServer: public Server
{
public:
    // message handler
    class Handler: public BaseObject {
    public:
        // ctor and dtor
        Handler(): BaseObject(true) { }
        virtual ~Handler() { };
    
        // message handler
        //
        // return == 0 - ok and continue
        // return < 0 - error and terminate
        // return > 0 - ok, but terminate
        virtual int operator()(EndPoint &serverep, Address &peer_address, void *buffer, int count) { return(1); };
    };

    // ctors and dtor
    MultiProcessDatagramServer(EndPoint &serverep, Handler &handler);
    virtual ~MultiProcessDatagramServer();

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    MultiProcessDatagramServer(const MultiProcessDatagramServer &src);
    MultiProcessDatagramServer &operator=(const MultiProcessDatagramServer &rhs);

protected:
    // track handlers
    EndPoint &serverep_;
    Handler &handler_;
};

class MultiProcessStreamServer: public Server
{
public:
    // message handler
    class Handler: public BaseObject {
    public:
        // ctor and dtor
        Handler(): BaseObject(true) { }
        virtual ~Handler() { }
    
        // message handler
        //
        // return == 0 - ok and continue
        // return < 0 - error and terminate
        // return > 0 - ok, but terminate
        virtual int operator()(EndPoint &peer) { return(1); }
    };

    // client process
    class SubServer: public Server
    {
    public:
        // ctors and dtor
        SubServer(EndPoint &subserverep, Handler &handler);
        virtual ~SubServer();

        // operations
        virtual int init();
        virtual int run();
        virtual int finish();

    protected:
        // data
        EndPoint &subserverep_;
        Handler &handler_;
    };

    // ctors and dtor
    MultiProcessStreamServer(EndPoint &serverep, Handler &handler);
    virtual ~MultiProcessStreamServer();

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    MultiProcessStreamServer(const MultiProcessStreamServer &src);
    MultiProcessStreamServer &operator=(const MultiProcessStreamServer &rhs);

protected:
    // track handlers
    EndPoint &serverep_;
    Handler &handler_;
};

}

#endif
