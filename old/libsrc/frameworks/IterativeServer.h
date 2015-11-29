#ifndef __OMBT_ITERATIVE_SERVER_H
#define __OMBT_ITERATIVE_SERVER_H

// iterative multi-service server

// system headers
#include <map>
#include <set>
#include <queue>

// local headers
#include <Debug.h>
#include <atomic/BaseObject.h>
#include <atomic/UseCntPtr.h>
#include <sockets/Address.h>
#include <sockets/EndPoint.h>
#include <timers/Timer.h>
#include <logging/Logging.h>

namespace ombt {

// iterative server for a connection
class IterativeServer: public BaseObject {
public:
    // reactor type
    enum MultiplexMode { None, Select, EPoll, Poll };
    enum { DefaultEPollSize = 128 };
    enum { DefaultEPollEventsToHandle = 128 };

    // service handlers base classes
    class BaseHandler: public BaseObject {
    public:
        // ctor and dtor
        BaseHandler();
        virtual ~BaseHandler();
    
        // set the server
        void setServer(IterativeServer *pserver);

    protected:
        UseCntPtr<IterativeServer> pserver_;
    };

    // handle reads, writes and i/o exceptions
    class BaseIOHandler: public BaseHandler {
    public:
        // ctor and dtor
        BaseIOHandler();
        virtual ~BaseIOHandler();
    
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
        virtual int handler(EndPoint *serverep);
    };

    // handle signals
    class BaseSignalHandler: public BaseHandler {
    public:
        // ctor and dtor
        BaseSignalHandler();
        virtual ~BaseSignalHandler();
    
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
        virtual int handler(EndPoint *serverep, int signalno);
    };

    // handle timers
    class BaseTimerHandler: public BaseHandler {
    public:
        // ctor and dtor
        BaseTimerHandler();
        virtual ~BaseTimerHandler();
    
	// return == 0 ==>> success and continue
	// return < 0 ==>> error and terminate
	// return > 0 ==>> success and close/remove end point.
        virtual int handler(EndPoint *serverep, Timer timer);
    };

    // ctors and dtor
    IterativeServer(MultiplexMode mode = Select);
    virtual ~IterativeServer();

    // register end-point handlers
    int registerReadHandler(EndPoint *pserver_ep, BaseIOHandler *phandler);
    int registerWriteHandler(EndPoint *pserver_ep, BaseIOHandler *phandler);
    int registerExceptionHandler(EndPoint *pserver_ep, BaseIOHandler *phandler);
    int registerSignalHandler(EndPoint *pserver_ep, BaseSignalHandler *phandler);

    // unregister end-point handlers
    int unregisterReadHandler(EndPoint *pserver_ep, BaseIOHandler *phandler);
    int unregisterWriteHandler(EndPoint *pserver_ep, BaseIOHandler *phandler);
    int unregisterExceptionHandler(EndPoint *pserver_ep, BaseIOHandler *phandler);
    int unregisterSignalHandler(EndPoint *ppserver_ep, BaseSignalHandler *phandler);

    // set a timer for an end point
    virtual void scheduleTimer(EndPoint *ep, Timer timer, BaseTimerHandler *handler);
    virtual void cancelTimer(EndPoint *ep, Timer timer, BaseTimerHandler *handler);

    // operations
    virtual int init();
    virtual int run();
    virtual int finish();

    // parameters
    int getEPollSize() const;
    void setEPollSize(int epollsize);

private:
    // leper colony
    IterativeServer(const IterativeServer &src);
    IterativeServer &operator=(const IterativeServer &rhs);

protected:
    // type of io multiplexing
    MultiplexMode mode_;
    int epollfd_;
    int epollsize_;

    // multipexors
    fd_set master_readfds_;
    fd_set master_writefds_;
    fd_set master_exceptionfds_;

    // track all the end points
    typedef std::map<int, UseCntPtr<EndPoint> > EP;
    typedef EP::iterator EPIter;
    typedef EP::const_iterator EPConstIter;
    EP endpoints_;

    // track handlers by socket
    typedef std::pair<UseCntPtr<EndPoint>, UseCntPtr<BaseIOHandler> > EPIOHPair;
    typedef std::map<int, EPIOHPair> IOHS;
    typedef IOHS::iterator IOHSIter;
    typedef IOHS::const_iterator IOHSConstIter;
    IOHS readhandlers_;
    IOHS writehandlers_;
    IOHS exceptionhandlers_;

    typedef std::pair<UseCntPtr<EndPoint>, UseCntPtr<BaseSignalHandler> > EPSHPair;
    typedef std::map<int, EPSHPair> SHS;
    typedef SHS::iterator SHSIter;
    typedef SHS::const_iterator SHSConstIter;
    SHS signalhandlers_;

    typedef std::pair<UseCntPtr<EndPoint>, UseCntPtr<BaseTimerHandler> > EPTHPair;
    typedef std::map<int, EPTHPair> THS;
    typedef THS::iterator THSIter;
    typedef THS::const_iterator THSConstIter;
    THS timerhandlers_;

    // timer queue
    class TimerQueueItem: public BaseObject {
    public:
        // ctors and dtor
        TimerQueueItem();
        TimerQueueItem(Timer timer, EndPoint *pep, BaseTimerHandler *phandler);
        TimerQueueItem(const TimerQueueItem &);
        ~TimerQueueItem();

        // assignment
        TimerQueueItem &operator=(const TimerQueueItem &);

        // comparison
        bool operator==(const TimerQueueItem &qi) const;
        bool operator!=(const TimerQueueItem &qi) const;
        bool operator<(const TimerQueueItem &qi) const;
        bool operator>(const TimerQueueItem &qi) const;
        bool operator<=(const TimerQueueItem &qi) const;
        bool operator>=(const TimerQueueItem &qi) const;

    public:
        // data
        Timer timer_;
        UseCntPtr<EndPoint> pendpoint_;
        UseCntPtr<BaseTimerHandler> phandler_;
    };

    class CanceledTimerQueueItem: public BaseObject {
    public:
        // ctors and dtor
        CanceledTimerQueueItem();
        CanceledTimerQueueItem(const TimerQueueItem &);
        CanceledTimerQueueItem(const CanceledTimerQueueItem &);
        ~CanceledTimerQueueItem();

        // assignment
        CanceledTimerQueueItem &operator=(const TimerQueueItem &);
        CanceledTimerQueueItem &operator=(const CanceledTimerQueueItem &);

        // comparison
        bool operator==(const CanceledTimerQueueItem &qi) const;
        bool operator!=(const CanceledTimerQueueItem &qi) const;
        bool operator<(const CanceledTimerQueueItem &qi) const;
        bool operator>(const CanceledTimerQueueItem &qi) const;
        bool operator<=(const CanceledTimerQueueItem &qi) const;
        bool operator>=(const CanceledTimerQueueItem &qi) const;

    public:
        // data
        TimerQueueItem canceleditem_;
    };

    typedef std::priority_queue<TimerQueueItem, std::vector<TimerQueueItem>, std::greater<TimerQueueItem> > TMRQ;
    TMRQ timerqueue_;

    typedef std::set<CanceledTimerQueueItem> CTMRS;
    typedef CTMRS::iterator CTMRSIter;
    CTMRS canceledtimers_;

protected:
    // utility functions
    int selectLoop();
    int epollLoop();
    int registerIOHandler(IOHS &handlers, fd_set &masterfds, 
                          EndPoint *pserver_ep, BaseIOHandler *phandler);
    int unregisterIOHandler(IOHS &handlers, fd_set &master_fds, EndPoint *pserver_ep, 
                            BaseIOHandler *phandler);
    void initfds(fd_set &fds, fd_set &masterfds, IOHS &handlers, int &maxsocket);
    void cleanup(int socket, fd_set &master_fds, fd_set &master_fds2, 
                 fd_set &master_fds3, EndPoint *pendpoint);
    int callIOhandler(int socket, fd_set &master_fds, IOHS &handlers,
                      fd_set &master_fds2, fd_set &master_fds3);
    int calltimerhandler(TimerQueueItem tqi, fd_set &master_fds,
                         fd_set &master_fds2, fd_set &master_fds3);
    void unregisterAllHandlers(EndPoint *pendpoint);
};

}

#endif
