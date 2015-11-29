// iterative multi-service server

// local headers
#include <IterativeServer.h>

namespace ombt {

// iterative server for a connection base handler classes
IterativeServer::BaseHandler::BaseHandler(): 
    BaseObject(true), pserver_(NULL) 
{
    // do nothing
}

IterativeServer::BaseHandler::~BaseHandler() 
{
    // do nothing
}
void 
IterativeServer::BaseHandler::setServer(IterativeServer *pserver) 
{
    pserver_ = pserver;
} 

// handle reads, writes and i/o exceptions
IterativeServer::BaseIOHandler::BaseIOHandler():
    BaseHandler()
{
    // do nothing
}

IterativeServer::BaseIOHandler::~BaseIOHandler()
{
    // do nothing
}

int
IterativeServer::BaseIOHandler::handler(EndPoint *serverep)
{
    return(1);
}

// handle signals
IterativeServer::BaseSignalHandler::BaseSignalHandler():
    BaseHandler()
{
    // do nothing
}

IterativeServer::BaseSignalHandler::~BaseSignalHandler()
{
    // do nothing
}

int
IterativeServer::BaseSignalHandler::handler(
    EndPoint *serverep, int signalno)
{
    return(1);
}

// handle timers
IterativeServer::BaseTimerHandler::BaseTimerHandler():
    BaseHandler()
{
    // do nothing
}

IterativeServer::BaseTimerHandler::~BaseTimerHandler()
{
    // do nothing
}

int
IterativeServer::BaseTimerHandler::handler(
    EndPoint *serverep, Timer timer)
{
    return(1);
}

// timer queue item
IterativeServer::TimerQueueItem::TimerQueueItem():
    BaseObject(), timer_(), pendpoint_(), phandler_()
{
    // do nothing
}

IterativeServer::TimerQueueItem::TimerQueueItem(
    Timer timer, EndPoint *pep, IterativeServer::BaseTimerHandler *phandler):
    BaseObject(), timer_(timer), pendpoint_(pep), phandler_(phandler)
{
    // do nothing
}

IterativeServer::TimerQueueItem::TimerQueueItem(const TimerQueueItem &tqi):
    BaseObject(), timer_(tqi.timer_), pendpoint_(tqi.pendpoint_), phandler_(tqi.phandler_)
{
    // do nothing
}

IterativeServer::TimerQueueItem::~TimerQueueItem()
{
    // do nothing
}

// operators
IterativeServer::TimerQueueItem &
IterativeServer::TimerQueueItem::operator=(const TimerQueueItem &rhs)
{
    if (this != &rhs)
    {
        BaseObject::operator=(rhs);
        timer_ = rhs.timer_;
        pendpoint_ = rhs.pendpoint_;
        phandler_ = rhs.phandler_;
    }
    return(*this);
}

bool
IterativeServer::TimerQueueItem::operator==(const TimerQueueItem &rhs) const
{
    return(timer_ == rhs.timer_);
}

bool
IterativeServer::TimerQueueItem::operator!=(const TimerQueueItem &rhs) const
{
    return(timer_ != rhs.timer_);
}

bool
IterativeServer::TimerQueueItem::operator<=(const TimerQueueItem &rhs) const
{
    return(timer_ <= rhs.timer_);
}

bool
IterativeServer::TimerQueueItem::operator>=(const TimerQueueItem &rhs) const
{
    return(timer_ >= rhs.timer_);
}

bool
IterativeServer::TimerQueueItem::operator<(const TimerQueueItem &rhs) const
{
    return(timer_ < rhs.timer_);
}

bool
IterativeServer::TimerQueueItem::operator>(const TimerQueueItem &rhs) const
{
    return(timer_ > rhs.timer_);
}

// canceled timer queue item
IterativeServer::CanceledTimerQueueItem::CanceledTimerQueueItem():
    BaseObject(), canceleditem_()
{
    // do nothing
}

IterativeServer::CanceledTimerQueueItem::CanceledTimerQueueItem(
    const IterativeServer::TimerQueueItem &tqi):
    BaseObject(), canceleditem_(tqi)
{
    // do nothing
}

IterativeServer::CanceledTimerQueueItem::CanceledTimerQueueItem(
    const CanceledTimerQueueItem &ctqi):
    BaseObject(), canceleditem_(ctqi.canceleditem_)
{
    // do nothing
}

IterativeServer::CanceledTimerQueueItem::~CanceledTimerQueueItem()
{
    // do nothing
}

// operators
IterativeServer::CanceledTimerQueueItem &
IterativeServer::CanceledTimerQueueItem::operator=(const TimerQueueItem &rhs)
{
    canceleditem_ = rhs;
    return(*this);
}

IterativeServer::CanceledTimerQueueItem &
IterativeServer::CanceledTimerQueueItem::operator=(const CanceledTimerQueueItem &rhs)
{
    if (this != &rhs)
    {
        canceleditem_ = rhs.canceleditem_;
    }
    return(*this);
}

bool
IterativeServer::CanceledTimerQueueItem::operator==(const CanceledTimerQueueItem &rhs) const
{
    return((canceleditem_.timer_ == rhs.canceleditem_.timer_) && 
           (canceleditem_.pendpoint_ == rhs.canceleditem_.pendpoint_) && 
           (canceleditem_.phandler_ == rhs.canceleditem_.phandler_));
}

bool
IterativeServer::CanceledTimerQueueItem::operator!=(const CanceledTimerQueueItem &rhs) const
{
    return(!(*this == rhs));
}

bool
IterativeServer::CanceledTimerQueueItem::operator<=(const CanceledTimerQueueItem &rhs) const
{
    return(!(*this > rhs));
}

bool
IterativeServer::CanceledTimerQueueItem::operator>=(const CanceledTimerQueueItem &rhs) const
{
    return(!(*this < rhs));
}

bool
IterativeServer::CanceledTimerQueueItem::operator<(const CanceledTimerQueueItem &rhs) const
{
    if (canceleditem_.timer_ < rhs.canceleditem_.timer_)
        return(true);
    else if (canceleditem_.timer_ > rhs.canceleditem_.timer_)
        return(false);
    else if (canceleditem_.pendpoint_ < rhs.canceleditem_.pendpoint_)
        return(true);
    else if (canceleditem_.pendpoint_ > rhs.canceleditem_.pendpoint_)
        return(false);
    else if (canceleditem_.phandler_ < rhs.canceleditem_.phandler_)
        return(true);
    else
        return(false);
}

bool
IterativeServer::CanceledTimerQueueItem::operator>(const CanceledTimerQueueItem &rhs) const
{
    if (canceleditem_.timer_ > rhs.canceleditem_.timer_)
        return(true);
    else if (canceleditem_.timer_ < rhs.canceleditem_.timer_)
        return(false);
    else if (canceleditem_.pendpoint_ > rhs.canceleditem_.pendpoint_)
        return(true);
    else if (canceleditem_.pendpoint_ > rhs.canceleditem_.pendpoint_)
        return(false);
    else if (canceleditem_.phandler_ > rhs.canceleditem_.phandler_)
        return(true);
    else
        return(false);
}

// ctors and dtor
IterativeServer::IterativeServer(MultiplexMode mode):
    BaseObject(true), 
    master_readfds_(), master_writefds_(), master_exceptionfds_(),
    mode_(mode), epollfd_(-1), epollsize_(-1), 
    endpoints_(), readhandlers_(), writehandlers_(), exceptionhandlers_(),
    signalhandlers_(), timerhandlers_(),
    timerqueue_(), canceledtimers_()
{
    FD_ZERO(&master_readfds_);
    FD_ZERO(&master_writefds_);
    FD_ZERO(&master_exceptionfds_);
}

IterativeServer::~IterativeServer()
{
}

// register and unregister end-point handlers
int
IterativeServer::registerIOHandler(IOHS &handlers, fd_set &masterfds, 
                                   EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    MustBeTrue(ok_);
    MustBeTrue(pserver_ep != NULL);
    MustBeTrue(phandler != NULL);

    UseCntPtr<EndPoint> pep(pserver_ep);
    endpoints_.insert(std::pair<int, UseCntPtr<EndPoint> >(pep->getSocket(), pep));
    FD_SET(pep->getSocket(), &masterfds);

    UseCntPtr<BaseIOHandler> ph(phandler);
    ph->setServer(this);

    handlers.insert(std::pair<int, EPIOHPair>(pep->getSocket(), EPIOHPair(pep, ph)));

    return 0;
}
int
IterativeServer::registerReadHandler(EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    return registerIOHandler(readhandlers_, master_readfds_, pserver_ep, phandler);
}

int
IterativeServer::registerWriteHandler(EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    return registerIOHandler(writehandlers_, master_writefds_, pserver_ep, phandler);
}

int
IterativeServer::registerExceptionHandler(EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    return registerIOHandler(exceptionhandlers_, master_exceptionfds_, pserver_ep, phandler);
}

int
IterativeServer::unregisterIOHandler(IOHS &handlers, fd_set &master_fds, 
                                     EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    MustBeTrue(ok_);
    MustBeTrue(pserver_ep != NULL);
    MustBeTrue(phandler != NULL);

    UseCntPtr<EndPoint> pep(pserver_ep);
    UseCntPtr<BaseIOHandler> ph(phandler);

    IOHSIter iohsit = handlers.find(pep->getSocket());
    if ((iohsit != handlers.end()) &&
        (iohsit->second.second == ph))
    {
        FD_CLR(pep->getSocket(), &master_fds);
        handlers.erase(iohsit);
        return 0;
    }
    else 
        return -1;
}

int
IterativeServer::unregisterReadHandler(EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    return unregisterIOHandler(readhandlers_, master_readfds_, pserver_ep, phandler);
}

int
IterativeServer::unregisterWriteHandler(EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    return unregisterIOHandler(writehandlers_, master_writefds_, pserver_ep, phandler);
}

int
IterativeServer::unregisterExceptionHandler(EndPoint *pserver_ep, BaseIOHandler *phandler)
{
    return unregisterIOHandler(exceptionhandlers_, master_exceptionfds_, pserver_ep, phandler);
}

int
IterativeServer::registerSignalHandler(EndPoint *pserver_ep, BaseSignalHandler *phandler)
{
    MustBeTrue(ok_);
    MustBeTrue(pserver_ep != NULL);
    MustBeTrue(phandler != NULL);

    UseCntPtr<EndPoint> pep(pserver_ep);
    endpoints_.insert(std::pair<int, UseCntPtr<EndPoint> >(pep->getSocket(), pep));
    // FD_SET(pep->getSocket(), &master_signalfds_);

    UseCntPtr<BaseSignalHandler> ph(phandler);
    ph->setServer(this);

    signalhandlers_.insert(std::pair<int, EPSHPair>(pep->getSocket(), EPSHPair(pep, ph)));

    return 0;
}

int
IterativeServer::unregisterSignalHandler(EndPoint *pserver_ep, BaseSignalHandler *phandler)
{
    MustBeTrue(ok_);
    MustBeTrue(pserver_ep != NULL);
    MustBeTrue(phandler != NULL);

    UseCntPtr<EndPoint> pep(pserver_ep);
    UseCntPtr<BaseSignalHandler> ph(phandler);

    SHSIter shsit = signalhandlers_.find(pep->getSocket());
    if ((shsit != signalhandlers_.end()) &&
        (shsit->second.second == ph))
    {
        // FD_CLR(pep->getSocket(), &master_signalfds_);
        signalhandlers_.erase(shsit);
        return 0;
    }
    else 
        return -1;
}

// set a timer for an end point
void
IterativeServer::scheduleTimer(EndPoint *pep, Timer timer, BaseTimerHandler *phandler)
{
    MustBeTrue(ok_);
    MustBeTrue(pep != NULL);
    MustBeTrue(phandler != NULL);
    TimerQueueItem tqi(timer, pep, phandler);
    timerqueue_.push(tqi);
}

void
IterativeServer::cancelTimer(EndPoint *pep, Timer timer, BaseTimerHandler *phandler)
{
    MustBeTrue(ok_);
    MustBeTrue(pep != NULL);
    MustBeTrue(phandler != NULL);
    CanceledTimerQueueItem ctqi(TimerQueueItem(timer, pep, phandler));
    canceledtimers_.insert(ctqi);
}

// operations
int
IterativeServer::init()
{
    return 0;
}

int
IterativeServer::run()
{
    MustBeTrue(ok_);
    switch (mode_)
    {
    case Select:
        return(selectLoop());
    case EPoll:
        return(epollLoop());
    default:
        return(-1);
    }
}

int
IterativeServer::finish()
{
    return 0;
}

// parameters
int
IterativeServer::getEPollSize() const
{
    return(epollsize_);
}

void
IterativeServer::setEPollSize(int epollsize)
{
    epollsize_ = epollsize;
}

// multipexors
void
IterativeServer::initfds(fd_set &fds, fd_set &masterfds, IOHS &handlers, int &maxsocket)
{
    FD_ZERO(&fds);
    FD_ZERO(&masterfds);

    IOHSIter ioit = handlers.begin();
    IOHSIter ioitend = handlers.end();
    for ( ; ioit != ioitend; ++ioit)
    {
        int socket = ioit->first;
        if (socket > maxsocket) maxsocket = socket;
        FD_SET(socket, &masterfds);
    }
}

void
IterativeServer::unregisterAllHandlers(EndPoint *pendpoint)
{
    UseCntPtr<EndPoint> pep(pendpoint);
    int socket = pep->getSocket();

    FD_CLR(socket, &master_readfds_);
    FD_CLR(socket, &master_writefds_);
    FD_CLR(socket, &master_exceptionfds_);
    //FD_CLR(socket, &master_signalfds_);

    endpoints_.erase(socket);
    readhandlers_.erase(socket);
    writehandlers_.erase(socket);
    exceptionhandlers_.erase(socket);
    signalhandlers_.erase(socket);
}

void
IterativeServer::cleanup(int socket, fd_set &master_fds, fd_set &master_fds2, 
                         fd_set &master_fds3, EndPoint *pendpoint)
{
    UseCntPtr<EndPoint> pep(pendpoint);

    FD_CLR(socket, &master_fds);
    FD_CLR(socket, &master_fds2);
    FD_CLR(socket, &master_fds3);
    unregisterAllHandlers(pep);
    pep->close();
}

int IterativeServer::callIOhandler(int socket, fd_set &master_fds, IOHS &handlers, 
                                   fd_set &master_fds2, fd_set &master_fds3)
{
DBGTRACE();
    IOHS::iterator ioit = handlers.find(socket);
    if (ioit == handlers.end())
    {
DBGTRACE();
        return(0);
    }

    UseCntPtr<EndPoint> pendpoint = ioit->second.first;
    UseCntPtr<BaseIOHandler> phandler = ioit->second.second;

DBGTRACE();
    int status = phandler->handler(pendpoint);
DBGPRINTF("callIOHandler: status = %d\n", status);
    if (status > 0)
    {
DBGTRACE();
        // success and done with this server
        cleanup(socket, master_fds, master_fds2, master_fds3, pendpoint);
    }
    else if (status < 0)
    {
DBGTRACE();
        // some type of error. for now, close endpoint
        cleanup(socket, master_fds, master_fds2, master_fds3, pendpoint);
    }
DBGTRACE();
    return(status);
}

int
IterativeServer::calltimerhandler(TimerQueueItem tqi, fd_set &master_fds, 
                                  fd_set &master_fds2, fd_set &master_fds3)
{
    int status = tqi.phandler_->handler(tqi.pendpoint_, tqi.timer_);
    if (status > 0)
    {
        // success and done with this server
        cleanup(tqi.pendpoint_->getSocket(), master_fds, master_fds2, master_fds3, tqi.pendpoint_);
    }
    else if (status < 0)
    {
        // some type of error. for now, close endpoint
        cleanup(tqi.pendpoint_->getSocket(), master_fds, master_fds2, master_fds3, tqi.pendpoint_);
    }
}

int 
IterativeServer::selectLoop()
{
    MustBeTrue(ok_);
    MustBeTrue(!endpoints_.empty());

    int maxsocket = -1;

    fd_set readfds;
    initfds(readfds, master_readfds_, readhandlers_, maxsocket);

    fd_set writefds;
    initfds(writefds, master_writefds_, writehandlers_, maxsocket);

    fd_set exceptionfds;
    initfds(exceptionfds, master_exceptionfds_, exceptionhandlers_, maxsocket);

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // reset descriptor list
        fd_set *preadfds;
        fd_set *pwritefds;
        fd_set *pexceptionfds;
        if (!readhandlers_.empty())
        {
            memcpy(&readfds, &master_readfds_, sizeof(master_readfds_));
            preadfds = &readfds;
        }
        else
        {
            preadfds = NULL;
        }
        if (!writehandlers_.empty())
        {
            memcpy(&writefds, &master_writefds_, sizeof(master_writefds_));
            pwritefds = &writefds;
        }
        else
        {
            pwritefds = NULL;
        }
        if (!exceptionhandlers_.empty())
        {
            memcpy(&exceptionfds, &master_exceptionfds_, sizeof(master_exceptionfds_));
            pexceptionfds = &exceptionfds;
        }
        else
        {
            pexceptionfds = NULL;
        }

        // do we have any timers?
        timeval *ptimer, timer;
        for (ptimer=NULL; !timerqueue_.empty(); )
        {
            TimerQueueItem tqi = timerqueue_.top();
            CTMRSIter ctit = canceledtimers_.find(CanceledTimerQueueItem(tqi));
            if (ctit != canceledtimers_.end())
            {
                canceledtimers_.erase(ctit);
                continue;
            }
            
            Timer timeout = tqi.timer_;
            Timer now;
            now.setToNow();
            timeout -= now;
            if (timeout < zero) timeout = zero;
            timeout.setTimeval(timer);
            ptimer = &timer;
        }

        // wait for an event
        int status = ::select(maxsocket+1, preadfds, pwritefds, pexceptionfds, ptimer);
        if (status < 0)
        {
             // error of some type
             ok_ = false;
             return(-1);
        }
        else if (status == 0)
        {
            // did timer fire?
            if (ptimer == &timer)
            {
                // we have a timeout. check queue
                Timer now;
                now.setToNow();
                
                while (!timerqueue_.empty())
                {
                    if (timerqueue_.top().timer_ > now) break;

                    TimerQueueItem tqi = timerqueue_.top();
                    timerqueue_.pop();

                    calltimerhandler(tqi, master_readfds_, master_writefds_, 
                                     master_exceptionfds_);
                }
            }
        }
        else
        {
            // we have i/o to handle
            for (int socket=0; socket<=maxsocket; ++socket)
            {
                // skip if not ready
                if (preadfds != NULL && FD_ISSET(socket, preadfds))
                {
                    callIOhandler(socket, master_readfds_, readhandlers_, 
                                  master_writefds_, master_exceptionfds_);
                }
                if (pwritefds != NULL && FD_ISSET(socket, pwritefds))
                {
                    callIOhandler(socket, master_writefds_, 
                                  writehandlers_, master_readfds_, master_exceptionfds_);
                }
                if (pexceptionfds != NULL && FD_ISSET(socket, pexceptionfds))
                {
                    callIOhandler(socket, master_exceptionfds_, 
                                  exceptionhandlers_, master_readfds_, master_writefds_);
                }
            }
        }
    }

    return 0;
}

int 
IterativeServer::epollLoop()
{
#if 0
    MustBeTrue(ok_);
    MustBeTrue(!endpoints_.empty());

    epoll_event ev;
    ::memset(&ev,0,sizeof(ev));

    epoll_event events[DefaultEPollEventsToHandle];

    for (Timer2 zero(0,0); !endpoints_.empty(); )
    {
        // do we have any timers?
        int timer;
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer2 t2 = tit->first;
            Timer2 now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
        }
        else
        {
            timer = -1;
        }

        // wait for an event
        int eventnum = ::epoll_wait(epollfd_, events, 
                                    DefaultEPollEventsToHandle, timer);
        if (eventnum < 0)
        {
             // error of some type
             TMSG("epoll_wait failed");
             DUMP(errno);
             return(-1);
        }
        else if (eventnum == 0)
        {
            // we have a timeout. check queue
            TMRQIter titbegin = tmrqueue_.begin();
            TMRQIter titend = tmrqueue_.end();
            TMRQIter tit = tmrqueue_.begin();

            Timer2 now;
            now.setToNow();
        
            for ( ; tit != titend && tit->first <= now; ++tit)
            {
                Timer2 tmr = tit->second.first;
                EndPoint ep = tit->second.second;

                int socket = ep.getSocket();
                THS::iterator hit = timerhandlers_.find(socket);
                if (hit == timerhandlers_.end()) continue;
                TimerHandler *handler = hit->second.second;

                int status = (*handler)(ep, tmr);
                if (status > 0)
                {
                    // success and done with this server
                    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
                else if (status < 0)
                {
                    // some type of error. for now, close endpoint
                    TMSG("timer handler failed");
                    DUMP(ep); DUMP(socket); DUMP(errno);
                    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
            }

            tmrqueue_.erase(titbegin, tit);
        }
        else
        {
            // we have i/o to handle
            for (int ievent=0; ievent<eventnum; ++ievent)
            {
                // socket with an event
                int socket = events[ievent].data.fd;

                // handle i/o
                HS::iterator hit = handlers_.find(socket);
                if (hit == handlers_.end()) continue;
                EndPoint ep = hit->second.first;
                Handler *handler = hit->second.second;
                int status = (*handler)(ep);
                if (status > 0)
                {
                    // success and done with this server
                    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                    unregisterAllHandlers(ep);
	            ep.close();
                }
                else if (status < 0)
                {
                    // some type of error. for now, close endpoint
                    TMSG("i/o handler failed");
                    DUMP(ep); DUMP(socket); DUMP(errno);
                    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                    unregisterAllHandlers(ep);
	            ep.close();
                }
            }
        }
    }
#endif

    return 0;
}

}
