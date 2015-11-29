#ifndef __OMBT_ITERATIVE_SERVER_H
#define __OMBT_ITERATIVE_SERVER_H

// iterative single-service server

// headers
#include <stdio.h>
#include <servers/Server.h>
#include <sockets/Address.h>
#include <sockets/EndPoint.h>

namespace ombt {

// iterative server for a connection
class IterativeDatagramServer: public Server
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
    IterativeDatagramServer(EndPoint &serverep, Handler &handler);
    virtual ~IterativeDatagramServer();

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    IterativeDatagramServer(const IterativeDatagramServer &src);
    IterativeDatagramServer &operator=(const IterativeDatagramServer &rhs);

protected:
    // track handlers
    EndPoint &serverep_;
    Handler &handler_;
};

class IterativeStreamServer: public Server
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

    // ctors and dtor
    IterativeStreamServer(EndPoint &serverep, Handler &handler);
    virtual ~IterativeStreamServer();

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    IterativeStreamServer(const IterativeStreamServer &src);
    IterativeStreamServer &operator=(const IterativeStreamServer &rhs);

protected:
    // track handlers
    EndPoint &serverep_;
    Handler &handler_;
};

}

#endif
