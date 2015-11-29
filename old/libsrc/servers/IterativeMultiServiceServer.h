#ifndef __OMBT_ITERATIVE_MULTI_SERVICE_SERVER_H
#define __OMBT_ITERATIVE_MULTI_SERVICE_SERVER_H

// iterative multi-service server

// system headers
#include <map>

// local headers
#include <Debug.h>
#include <servers/Server.h>
#include <sockets/Address.h>
#include <sockets/EndPoint.h>
#include <timers/Timer.h>
#include <adt/Tuple.h>
#include <adt/PriorityQueue_Array.h>
#include <logging/Logging.h>

namespace ombt {

#define USESTL 0

// iterative server for a connection
class IterativeMSDatagramServer: public Server
{
public:
    // message handler
    class Handler: public BaseObject {
    public:
        // ctor and dtor
        Handler(): BaseObject(true), server_(NULL) { }
        virtual ~Handler() { };
    
        // message handler
	//
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
	//
        virtual int operator()(EndPoint &serverep) { return(1); };

        // set the server
        void setServer(Server *server) { server_ = server; } 

    protected:
        Server *server_;
    };

    // timer handler
    class TimerHandler: public BaseObject {
    public:
        // ctor and dtor
        TimerHandler(): BaseObject(true), server_(NULL) { }
        virtual ~TimerHandler() { };
    
        // timer handler
	//
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
	//
        virtual int operator()(EndPoint &serverep, Timer &timer) { return(1); };

        // set the server
        void setServer(Server *server) { server_ = server; } 

    protected:
        Server *server_;
    };

    // ctors and dtor
    IterativeMSDatagramServer(MultiplexMode mode = Select);
    virtual ~IterativeMSDatagramServer();

    // register and unregister end-point handlers
    int registerEndPoint(EndPoint &server_ep);
    int unregisterEndPoint(EndPoint &server_ep);
    int registerHandler(EndPoint &server_ep, Handler &handler);
    int registerTimerHandler(EndPoint &server_ep, TimerHandler &handler);
    int unregisterHandler(EndPoint &server_ep);
    int unregisterTimerHandler(EndPoint &server_ep);
    int unregisterAllHandlers(EndPoint &server_ep);

    // set a timer for an end point
    virtual void scheduleTimer(EndPoint &myep, Timer &timer);
    virtual void cancelTimer(EndPoint &myep, Timer &timer);

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

    // parameters
    int getEPollSize() const { return(epollsize_); }
    void setEPollSize(int epollsize) { epollsize_ = epollsize; }

private:
    // leper colony
    IterativeMSDatagramServer(const IterativeMSDatagramServer &src);
    IterativeMSDatagramServer &operator=(const IterativeMSDatagramServer &rhs);

protected:
    // type of io multiplexing
    Server::MultiplexMode mode_;
    int epollfd_;
    int epollsize_;

    // multipexors
    int selectLoop();
    int epollLoop();

    // track all the end points
    typedef std::map<int, EndPoint> EP;
    typedef EP::iterator EPIter;
    typedef EP::const_iterator EPConstIter;
    EP endpoints_;

    // track handlers by socket
    typedef std::map<int, std::pair<EndPoint, Handler * > > HS;
    typedef HS::iterator HSIter;
    typedef HS::const_iterator HSConstIter;
    HS handlers_;

    typedef std::map<int, std::pair<EndPoint, TimerHandler * > > THS;
    typedef THS::iterator THSIter;
    typedef THS::const_iterator THSConstIter;
    THS timerhandlers_;

    // timer queue
#if USESTL
    typedef std::multimap<Timer, std::pair<Timer, EndPoint> > TMRQ;
    typedef TMRQ::iterator TMRQIter;
    typedef TMRQ::const_iterator TMRQConstIter;
#else
    typedef PriorityQueue_Array< Tuple<Timer, Tuple<Timer, EndPoint> > > TMRQ;
#endif
    TMRQ tmrqueue_;
};

class IterativeMSStreamServer: public Server
{
public:
    // message handler for server endpoints AND accepted endpoints
    class Handler: public BaseObject {
    public:
        // ctor and dtor
        Handler(): BaseObject(true), server_(NULL) { }
        virtual ~Handler() { };
    
        // client handler
	//
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
	//
        virtual int operator()(EndPoint &acceptep) { return(1); };

        // set the server
        void setServer(Server *server) { server_ = server; } 

    protected:
        Server *server_;
    };

    // timer handler
    class TimerHandler: public BaseObject {
    public:
        // ctor and dtor
        TimerHandler(): BaseObject(true), server_(NULL) { }
        virtual ~TimerHandler() { };
    
        // timer handler
	//
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
	//
        virtual int operator()(EndPoint &acceptep, Timer &timer) { return(1); };

        // set the server
        void setServer(Server *server) { server_ = server; } 

    protected:
        Server *server_;
    };

    // ctors and dtor
    IterativeMSStreamServer(MultiplexMode mode = Select);
    virtual ~IterativeMSStreamServer();

    // register and unregister end-point handlers
    int registerEndPoint(EndPoint &server_ep);
    int unregisterEndPoint(EndPoint &server_ep);
    int registerHandler(EndPoint &server_ep, Handler &handler);
    int registerTimerHandler(EndPoint &server_ep, TimerHandler &handler);
    int unregisterHandler(EndPoint &server_ep);
    int unregisterTimerHandler(EndPoint &server_ep);
    int unregisterAllHandlers(EndPoint &server_ep);

    // set a timer for an end point
    virtual void scheduleTimer(EndPoint &myep, Timer &timer);
    virtual void cancelTimer(EndPoint &myep, Timer &timer);

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

    // parameters
    int getEPollSize() const { return(epollsize_); }
    void setEPollSize(int epollsize) { epollsize_ = epollsize; }

private:
    // leper colony
    IterativeMSStreamServer(const IterativeMSStreamServer &src);
    IterativeMSStreamServer &operator=(const IterativeMSStreamServer &rhs);

protected:
    // type of io multiplexing
    Server::MultiplexMode mode_;
    int epollfd_;
    int epollsize_;

    // multipexors
    int selectLoop();
    int epollLoop();

    // track all the end points
    typedef std::map<int, EndPoint> EP;
    typedef EP::iterator EPIter;
    typedef EP::const_iterator EPConstIter;
    EP endpoints_;

    // track handlers by socket
    typedef std::map<int, std::pair<EndPoint, Handler * > > HS;
    typedef HS::iterator HSIter;
    typedef HS::const_iterator HSConstIter;
    HS handlers_;

    typedef std::map<int, std::pair<EndPoint, TimerHandler * > > THS;
    typedef THS::iterator THSIter;
    typedef THS::const_iterator THSConstIter;
    THS timerhandlers_;

    // timer queue
#if USESTL
    typedef std::multimap<Timer, std::pair<Timer, EndPoint> > TMRQ;
    typedef TMRQ::iterator TMRQIter;
    typedef TMRQ::const_iterator TMRQConstIter;
#else
    typedef PriorityQueue_Array< Tuple<Timer, Tuple<Timer, EndPoint> > > TMRQ;
#endif
    TMRQ tmrqueue_;
};

}

#endif
