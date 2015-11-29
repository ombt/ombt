#ifndef __OMBT_MULTI_THREAD_SERVER_H
#define __OMBT_MULTI_THREAD_SERVER_H

// multithread single-service server

// headers
#include <stdio.h>
#include <servers/Server.h>
#include <sockets/EndPoint.h>
#include <threads/SimpleThread.h>

namespace ombt {

// threading server for a connection
class MultiThreadStreamServer: public Server
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
    MultiThreadStreamServer(EndPoint &serverep, Handler &handler);
    virtual ~MultiThreadStreamServer();

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

private:
    // leper colony
    MultiThreadStreamServer(const MultiThreadStreamServer &src);
    MultiThreadStreamServer &operator=(const MultiThreadStreamServer &rhs);

protected:
    // thread task to start up handler
    class Task: public SimpleThread::Task {
    public:
        // ctor and dtor
        Task(EndPoint &subserverep, Handler &handler);
        virtual ~Task();

        // worker functions
        virtual int init();
        virtual int run();
        virtual int finish();

    protected:
        // data
        EndPoint &subserverep_;
        Handler &handler_;
    };

    // track handlers
    EndPoint &serverep_;
    Handler &handler_;
};

}

#endif

