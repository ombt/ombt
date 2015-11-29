// iterative server class

// headers
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <set>
#include <Debug.h>
#include <atomic/UseCntPtr.h>
#include <MultiProcessMultiServiceServer.h>

namespace ombt {

// signal handler for death-of-child
static void
reaper(int sig)
{
    int status;
    while (::wait3(&status, WNOHANG, (struct rusage *)0) >= 0) ;
    (void) signal(SIGCHLD, reaper);
}

// client server process
MultiProcMSDatagramServer::SubServer::SubServer(MultiplexMode mode):
        Server(), mode_(mode), epollfd_(-1), epollsize_(DefaultEPollSize), handlers_(), timerhandlers_(), tmrqueue_()
{
    ok_ = true;
}

MultiProcMSDatagramServer::SubServer::~SubServer()
{
    if (ok_)
    {
        EPIter epit = endpoints_.begin();
        EPIter epitend = endpoints_.end();
        for ( ; epit != epitend; ++epit)
        {
             EndPoint serverep = epit->second;
             serverep.close();
        }
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;
}

// register and unregister handlers
int
MultiProcMSDatagramServer::SubServer::registerEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.insert(std::pair<int, EndPoint>(ep.getSocket(), ep));

    return(0);
}

int
MultiProcMSDatagramServer::SubServer::registerHandler(EndPoint &ep, Handler &h)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    h.setServer(this);
    handlers_.insert(std::pair<int, std::pair<EndPoint, Handler * > >(
        ep.getSocket(), std::pair<EndPoint, Handler * >(ep, &h)));

    return(0);
}

int
MultiProcMSDatagramServer::SubServer::registerTimerHandler(EndPoint &ep, TimerHandler &th)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    th.setServer(this);
    timerhandlers_.insert(std::pair<int, std::pair<EndPoint, TimerHandler * > >(
        ep.getSocket(), std::pair<EndPoint, TimerHandler * >(ep, &th)));

    return(0);
}

int 
MultiProcMSDatagramServer::SubServer::unregisterEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSDatagramServer::SubServer::unregisterHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    handlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSDatagramServer::SubServer::unregisterTimerHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    timerhandlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSDatagramServer::SubServer::unregisterAllHandlers(EndPoint &ep)
{
    CheckReturn(ok_);

    unregisterEndPoint(ep);
    unregisterHandler(ep);
    unregisterTimerHandler(ep);

    return(0);
}

// set a timer for an end point
void 
MultiProcMSDatagramServer::SubServer::scheduleTimer(EndPoint &myep, Timer &timer)
{
    Timer qtmr(timer);
    Timer now;
    now.setToNow();
    qtmr += now;
#if USESTL
    tmrqueue_.insert(std::pair<Timer, std::pair<Timer, EndPoint> >(qtmr, std::pair<Timer, EndPoint>(timer, myep)));
#else
    tmrqueue_.enqueue(Tuple<Timer, Tuple<Timer, EndPoint> >(qtmr, Tuple<Timer, EndPoint>(timer, myep)));
#endif
}

void 
MultiProcMSDatagramServer::SubServer::cancelTimer(EndPoint &myep, Timer &timer)
{
#if 0
    TMRQIter tmrit = tmrqueue_.find(myep);
    TMRQIter tmritend = tmrqueue_.end();

    if (tmrit == tmritend) return;

    for ( ; tmrit != tmritend; ++tmrit)
    {
        if (tmrit->second == myep) tmrqueue_.erase(tmrit);
    }
#endif
}

// operations
int
MultiProcMSDatagramServer::SubServer::init()
{
    CheckReturn(ok_);

    if (mode_ != EPoll) return(0);

    CheckReturn(epollsize_ >= 0);
    epollfd_ = ::epoll_create(epollsize_);
    CheckReturn(epollfd_ >= 0);

    HSIter hit = handlers_.begin();
    HSIter hitend = handlers_.end();
    for ( ; hit != hitend; ++hit)
    {
        EndPoint serverep = hit->second.first;
        serverep.setNonBlocking(true);
        epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = serverep.getSocket();
        CheckReturn(::epoll_ctl(epollfd_, EPOLL_CTL_ADD, 
                                serverep.getSocket(), &ev) == 0);
    }

    return(0);
}

int
MultiProcMSDatagramServer::SubServer::run()
{
    CheckReturn(ok_);

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
MultiProcMSDatagramServer::SubServer::selectLoop()
{
    CheckReturn(ok_);
    CheckReturn(!handlers_.empty());

    fd_set readfds;
    FD_ZERO(&readfds);
    fd_set master_readfds;
    FD_ZERO(&master_readfds);

    int maxsocket = -1;
    EPIter epit = endpoints_.begin();
    EPIter epitend = endpoints_.end();
    for ( ; epit != epitend; ++epit)
    {
        int socket = epit->first;
        if (socket > maxsocket) maxsocket = socket;
        FD_SET(socket, &master_readfds);
    }

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // reset descriptor list
        memcpy(&readfds, &master_readfds, sizeof(master_readfds));

        // do we have any timers?
        timeval *ptimer, timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
        }
        else
        {
            ptimer = NULL;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
	}
	else
	{
            ptimer = NULL;
	}
#endif

        // wait for an event
        int status = ::select(maxsocket+1, &readfds, NULL, NULL, ptimer);
        if (status < 0)
        {
             // error of some type
             if (errno == EINTR) continue;
             TMSG("select failed");
             DUMP(errno);
             return(-1);
        }
        else if (status == 0)
        {
            // did timer fire?
            if (ptimer == &timer)
            {
#if USESTL
                // we have a timeout. check queue
                TMRQIter titbegin = tmrqueue_.begin();
                TMRQIter titend = tmrqueue_.end();
                TMRQIter tit = tmrqueue_.begin();

                Timer now;
                now.setToNow();
                
                for ( ; tit != titend && tit->first <= now; ++tit)
                {
                    Timer tmr = tit->second.first;
                    EndPoint ep = tit->second.second;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                }

                tmrqueue_.erase(titbegin, tit);
#else
                // we have a timeout. check queue
                Timer now;
                now.setToNow();
                
		while (!tmrqueue_.isEmpty())
		{
                    Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                    tmrqueue_.dequeue(qtimer);

                    if (qtimer.key > now)
                    {
                        tmrqueue_.enqueue(qtimer);
                        break;
                    }

                    Timer tmr = qtimer.data.key;
                    EndPoint ep = qtimer.data.data;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                }
#endif
            }
        }
        else
        {
            // we have i/o to handle
            for (int socket=0; socket<=maxsocket; ++socket)
            {
                // skip if not ready
                if (!FD_ISSET(socket, &readfds)) continue;
                // handle i/o
                HS::iterator hit = handlers_.find(socket);
                if (hit == handlers_.end()) continue;
                EndPoint ep = hit->second.first;
                Handler *handler = hit->second.second;
                int status = (*handler)(ep);
                if (status > 0)
                {
                    // success and done with this server
                    FD_CLR(socket, &master_readfds);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
                else if (status < 0)
                {
                    // some type of error. for now, close endpoint
                    TMSG("i/o handler failed");
                    DUMP(ep); DUMP(socket); DUMP(errno);
                    FD_CLR(socket, &master_readfds);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
            }
        }
    }

    return(0);
}

int
MultiProcMSDatagramServer::SubServer::epollLoop()
{
    CheckReturn(ok_);
    CheckReturn(!handlers_.empty());

    epoll_event ev;
    ::memset(&ev,0,sizeof(ev));

    epoll_event events[DefaultEPollEventsToHandle];

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // do we have any timers?
        int timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
        }
        else
        {
            timer = -1;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
	}
	else
	{
            timer = -1;
	}
#endif

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
#if USESTL
            // we have a timeout. check queue
            TMRQIter titbegin = tmrqueue_.begin();
            TMRQIter titend = tmrqueue_.end();
            TMRQIter tit = tmrqueue_.begin();

            Timer now;
            now.setToNow();
        
            for ( ; tit != titend && tit->first <= now; ++tit)
            {
                Timer tmr = tit->second.first;
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
#else
            // we have a timeout. check queue
            Timer now;
            now.setToNow();
            
            while (!tmrqueue_.isEmpty())
            {
                Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                tmrqueue_.dequeue(qtimer);

                if (qtimer.key > now)
                {
                    tmrqueue_.enqueue(qtimer);
                    break;
                }

                Timer tmr = qtimer.data.key;
                EndPoint ep = qtimer.data.data;

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
#endif
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

    return(0);
}

int
MultiProcMSDatagramServer::SubServer::finish()
{
    CheckReturn(ok_);

    if (mode_ == EPoll)
    {
        ::close(epollfd_);
        epollfd_ = -1;
    }

    EPIter epit = endpoints_.begin();
    EPIter epitend = endpoints_.end();
    for ( ; epit != epitend; ++epit)
    {
         EndPoint serverep = epit->second;
         serverep.close();
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;

    return(0);
}

// ctors and dtor
MultiProcMSDatagramServer::MultiProcMSDatagramServer(MultiplexMode mode):
        Server(), mode_(mode), epollfd_(-1), epollsize_(DefaultEPollSize), handlers_(), timerhandlers_(), tmrqueue_()
{
    ok_ = true;
}

MultiProcMSDatagramServer::~MultiProcMSDatagramServer()
{
    if (ok_)
    {
        EPIter epit = endpoints_.begin();
        EPIter epitend = endpoints_.end();
        for ( ; epit != epitend; ++epit)
        {
             EndPoint serverep = epit->second;
             serverep.close();
        }
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;
}

// register and unregister handlers
int
MultiProcMSDatagramServer::registerEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.insert(std::pair<int, EndPoint>(ep.getSocket(), ep));

    return(0);
}

int
MultiProcMSDatagramServer::registerHandler(EndPoint &ep, Handler &h)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    h.setServer(this);
    handlers_.insert(std::pair<int, std::pair<EndPoint, Handler * > >(
        ep.getSocket(), std::pair<EndPoint, Handler * >(ep, &h)));

    return(0);
}

int
MultiProcMSDatagramServer::registerTimerHandler(EndPoint &ep, TimerHandler &th)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    th.setServer(this);
    timerhandlers_.insert(std::pair<int, std::pair<EndPoint, TimerHandler * > >(
        ep.getSocket(), std::pair<EndPoint, TimerHandler * >(ep, &th)));

    return(0);
}

int 
MultiProcMSDatagramServer::unregisterEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSDatagramServer::unregisterHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    handlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSDatagramServer::unregisterTimerHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    timerhandlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSDatagramServer::unregisterAllHandlers(EndPoint &ep)
{
    CheckReturn(ok_);

    unregisterEndPoint(ep);
    unregisterHandler(ep);
    unregisterTimerHandler(ep);

    return(0);
}

// set a timer for an end point
void 
MultiProcMSDatagramServer::scheduleTimer(EndPoint &myep, Timer &timer)
{
    Timer qtmr(timer);
    Timer now;
    now.setToNow();
    qtmr += now;
#if USESTL
    tmrqueue_.insert(std::pair<Timer, std::pair<Timer, EndPoint> >(qtmr, std::pair<Timer, EndPoint>(timer, myep)));
#else
    tmrqueue_.enqueue(Tuple<Timer, Tuple<Timer, EndPoint> >(qtmr, Tuple<Timer, EndPoint>(timer, myep)));
#endif
}

void 
MultiProcMSDatagramServer::cancelTimer(EndPoint &myep, Timer &timer)
{
#if 0
    TMRQIter tmrit = tmrqueue_.find(myep);
    TMRQIter tmritend = tmrqueue_.end();

    if (tmrit == tmritend) return;

    for ( ; tmrit != tmritend; ++tmrit)
    {
        if (tmrit->second == myep) tmrqueue_.erase(tmrit);
    }
#endif
}

// operations
int
MultiProcMSDatagramServer::init()
{
    CheckReturn(ok_);

    if (mode_ == EPoll)
    {
        CheckReturn(epollsize_ >= 0);
        epollfd_ = ::epoll_create(epollsize_);
        CheckReturn(epollfd_ >= 0);
    }

    HSIter hit = handlers_.begin();
    HSIter hitend = handlers_.end();
    for ( ; hit != hitend; ++hit)
    {
        EndPoint serverep = hit->second.first;
        CheckReturn(serverep.setReuseAddr(true) == 0);
        CheckReturn(serverep.bind() == 0);

        if (mode_ == EPoll)
        {
            serverep.setNonBlocking(true);
            epoll_event ev;
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = serverep.getSocket();
            CheckReturn(::epoll_ctl(epollfd_, EPOLL_CTL_ADD, 
                                    serverep.getSocket(), &ev) == 0);
        }
    }

    return(0);
}

int
MultiProcMSDatagramServer::run()
{
    CheckReturn(ok_);

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
MultiProcMSDatagramServer::selectLoop()
{
    CheckReturn(ok_);
    CheckReturn(!handlers_.empty());

    (void) signal(SIGCHLD, reaper);

    fd_set readfds;
    FD_ZERO(&readfds);
    fd_set master_readfds;
    FD_ZERO(&master_readfds);

    int maxsocket = -1;
    EPIter epit = endpoints_.begin();
    EPIter epitend = endpoints_.end();
    for ( ; epit != epitend; ++epit)
    {
        int socket = epit->first;
        if (socket > maxsocket) maxsocket = socket;
        FD_SET(socket, &master_readfds);
    }

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // reset descriptor list
        memcpy(&readfds, &master_readfds, sizeof(master_readfds));

        // do we have any timers?
        timeval *ptimer, timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
        }
        else
        {
            ptimer = NULL;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
	}
	else
	{
            ptimer = NULL;
	}
#endif

        // wait for an event
        int status = ::select(maxsocket+1, &readfds, NULL, NULL, ptimer);
        if (status < 0)
        {
             // error of some type
             if (errno == EINTR) continue;
             TMSG("select failed");
             DUMP(errno);
             return(-1);
        }
        else if (status == 0)
        {
            // did timer fire?
            if (ptimer == &timer)
            {
#if USESTL
                // we have a timeout. check queue
                TMRQIter titbegin = tmrqueue_.begin();
                TMRQIter titend = tmrqueue_.end();
                TMRQIter tit = tmrqueue_.begin();

                Timer now;
                now.setToNow();
                
                for ( ; tit != titend && tit->first <= now; ++tit)
                {
                    Timer tmr = tit->second.first;
                    EndPoint ep = tit->second.second;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                }

                tmrqueue_.erase(titbegin, tit);
#else
                // we have a timeout. check queue
                Timer now;
                now.setToNow();
                
		while (!tmrqueue_.isEmpty())
		{
                    Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                    tmrqueue_.dequeue(qtimer);

                    if (qtimer.key > now)
                    {
                        tmrqueue_.enqueue(qtimer);
                        break;
                    }

                    Timer tmr = qtimer.data.key;
                    EndPoint ep = qtimer.data.data;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                }
#endif
            }
        }
        else
        {
            // we have i/o to handle
            for (int socket=0; socket<=maxsocket; ++socket)
            {
                // skip if not ready
                if (!FD_ISSET(socket, &readfds)) continue;
                // handle i/o
                HS::iterator hit = handlers_.find(socket);
                if (hit == handlers_.end()) continue;
                EndPoint ep = hit->second.first;
                Handler *handler = hit->second.second;

		int pid = ::fork();
		if (pid == 0)
		{
                    // the child -

                    // close all sockets not the child
                    EPIter epit = endpoints_.begin();
                    EPIter epitend = endpoints_.end();
                    for ( ; epit != epitend; ++epit)
                    {
                        if (epit->first == socket) continue;
                        EndPoint tmpep = epit->second;
                        unregisterAllHandlers(tmpep);
                        tmpep.close();
                    }

                    // create subserver and run it.
                    SubServer child(mode_);
                    child.registerEndPoint(ep);
                    child.registerHandler(ep, *handler);

                    // run the server
                    if (child.init() != 0) exit(-1);
                    if (child.run() != 0) exit(-1);
                    if (child.finish() != 0) exit(-1);

                    // all done
                    exit(0);
		}
		else if (pid < 0)
		{
                    // error in parent
                    return(-1);
		}
            }
        }
    }

    return(0);
}

int
MultiProcMSDatagramServer::epollLoop()
{
    CheckReturn(ok_);
    CheckReturn(!handlers_.empty());

    (void) signal(SIGCHLD, reaper);

    epoll_event ev;
    ::memset(&ev,0,sizeof(ev));

    epoll_event events[DefaultEPollEventsToHandle];

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // do we have any timers?
        int timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
        }
        else
        {
            timer = -1;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
	}
	else
	{
            timer = -1;
	}
#endif

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
#if USESTL
            // we have a timeout. check queue
            TMRQIter titbegin = tmrqueue_.begin();
            TMRQIter titend = tmrqueue_.end();
            TMRQIter tit = tmrqueue_.begin();

            Timer now;
            now.setToNow();
        
            for ( ; tit != titend && tit->first <= now; ++tit)
            {
                Timer tmr = tit->second.first;
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
#else
            // we have a timeout. check queue
            Timer now;
            now.setToNow();
            
            while (!tmrqueue_.isEmpty())
            {
                Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                tmrqueue_.dequeue(qtimer);

                if (qtimer.key > now)
                {
                    tmrqueue_.enqueue(qtimer);
                    break;
                }

                Timer tmr = qtimer.data.key;
                EndPoint ep = qtimer.data.data;

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
#endif
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

		int pid = ::fork();
		if (pid == 0)
		{
                    // the child -

                    // close all sockets not the child
                    EPIter epit = endpoints_.begin();
                    EPIter epitend = endpoints_.end();
                    for ( ; epit != epitend; ++epit)
                    {
                        if (epit->first == socket) continue;
                        EndPoint tmpep = epit->second;
                        unregisterAllHandlers(tmpep);
                        tmpep.close();
                    }

                    // create subserver and run it.
                    SubServer child(mode_);
                    child.registerEndPoint(ep);
                    child.registerHandler(ep, *handler);

                    // run the server
                    if (child.init() != 0) exit(-1);
                    if (child.run() != 0) exit(-1);
                    if (child.finish() != 0) exit(-1);

                    // all done
                    exit(0);
		}
		else if (pid < 0)
		{
                    // error in parent
                    return(-1);
		}
            }
        }
    }

    return(0);
}

int
MultiProcMSDatagramServer::finish()
{
    CheckReturn(ok_);

    if (mode_ == EPoll)
    {
        ::close(epollfd_);
        epollfd_ = -1;
    }

    EPIter epit = endpoints_.begin();
    EPIter epitend = endpoints_.end();
    for ( ; epit != epitend; ++epit)
    {
         EndPoint serverep = epit->second;
         serverep.close();
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;

    return(0);
}

// ctors and dtor
MultiProcMSStreamServer::SubServer::SubServer(MultiplexMode mode):
        Server(), mode_(mode), epollfd_(-1), epollsize_(DefaultEPollSize), 
        handlers_(), timerhandlers_(), tmrqueue_()
{
    ok_ = true;
}

MultiProcMSStreamServer::SubServer::~SubServer()
{
    if (ok_)
    {
        EPIter epit = endpoints_.begin();
        EPIter epitend = endpoints_.end();
        for ( ; epit != epitend; ++epit)
        {
             EndPoint serverep = epit->second;
             serverep.close();
        }
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;
}

// register and unregister handlers
int
MultiProcMSStreamServer::SubServer::registerEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.insert(std::pair<int, EndPoint>(ep.getSocket(), ep));

    return(0);
}

int
MultiProcMSStreamServer::SubServer::registerHandler(EndPoint &ep, Handler &h)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    h.setServer(this);
    handlers_.insert(std::pair<int, std::pair<EndPoint, Handler * > >(
        ep.getSocket(), std::pair<EndPoint, Handler * >(ep, &h)));

    return(0);
}

int
MultiProcMSStreamServer::SubServer::registerTimerHandler(EndPoint &ep, TimerHandler &th)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    th.setServer(this);
    timerhandlers_.insert(std::pair<int, std::pair<EndPoint, TimerHandler * > >(
        ep.getSocket(), std::pair<EndPoint, TimerHandler * >(ep, &th)));

    return(0);
}

int 
MultiProcMSStreamServer::SubServer::unregisterEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSStreamServer::SubServer::unregisterHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    handlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSStreamServer::SubServer::unregisterTimerHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    timerhandlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSStreamServer::SubServer::unregisterAllHandlers(EndPoint &ep)
{
    CheckReturn(ok_);

    unregisterEndPoint(ep);
    unregisterHandler(ep);
    unregisterTimerHandler(ep);

    return(0);
}

// set a timer for an end point
void 
MultiProcMSStreamServer::SubServer::scheduleTimer(EndPoint &myep, Timer &timer)
{
    Timer qtmr(timer);
    Timer now;
    now.setToNow();
    qtmr += now;
#if USESTL
    tmrqueue_.insert(std::pair<Timer, std::pair<Timer, EndPoint> >(qtmr, std::pair<Timer, EndPoint>(timer, myep)));
#else
    tmrqueue_.enqueue(Tuple<Timer, Tuple<Timer, EndPoint> >(qtmr, Tuple<Timer, EndPoint>(timer, myep)));
#endif
}

void 
MultiProcMSStreamServer::SubServer::cancelTimer(EndPoint &myep, Timer &timer)
{
#if 0
    TMRQIter tmrit = tmrqueue_.find(myep);
    TMRQIter tmritend = tmrqueue_.end();

    if (tmrit == tmritend) return;

    for ( ; tmrit != tmritend; ++tmrit)
    {
        if (tmrit->second == myep) tmrqueue_.erase(tmrit);
    }
#endif
}

// operations
int
MultiProcMSStreamServer::SubServer::init()
{
    CheckReturn(ok_);

    if (mode_ != EPoll) return(0);

    CheckReturn(epollsize_ >= 0);
    epollfd_ = ::epoll_create(epollsize_);
    CheckReturn(epollfd_ >= 0);

    HSIter hit = handlers_.begin();
    HSIter hitend = handlers_.end();
    for ( ; hit != hitend; ++hit)
    {
        EndPoint serverep = hit->second.first;
        serverep.setNonBlocking(true);
        epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = serverep.getSocket();
        CheckReturn(::epoll_ctl(epollfd_, EPOLL_CTL_ADD, 
                                serverep.getSocket(), &ev) == 0);
    }

    return(0);
}

int
MultiProcMSStreamServer::SubServer::run()
{
    CheckReturn(ok_);

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
MultiProcMSStreamServer::SubServer::selectLoop()
{
    CheckReturn(ok_);
    CheckReturn(!endpoints_.empty());

    fd_set readfds;
    FD_ZERO(&readfds);
    fd_set exceptfds;
    FD_ZERO(&exceptfds);
    fd_set master_readfds;
    FD_ZERO(&master_readfds);

    int maxsocket = -1;
    HSIter hit = handlers_.begin();
    HSIter hitend = handlers_.end();
    for ( ; hit != hitend; ++hit)
    {
        int socket = hit->first;
        if (socket > maxsocket) maxsocket = socket;
        FD_SET(socket, &master_readfds);
    }

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        memcpy(&readfds, &master_readfds, sizeof(master_readfds));
        memcpy(&exceptfds, &master_readfds, sizeof(master_readfds));

        // do we have any timers?
        timeval *ptimer, timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
        }
        else
        {
            ptimer = NULL;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
	}
	else
	{
            ptimer = NULL;
	}
#endif

        int status = ::select(maxsocket+1, &readfds, NULL, &exceptfds, ptimer);
        if (status < 0)
        {
            if (errno == EINTR) continue;
            TMSG("select failed");
            DUMP(errno);
            return(-1);
        }
        else if (status == 0)
        {
            // did timer fire?
            if (ptimer == &timer)
            {
                // we have a timeout. check queue
#if USESTL
                TMRQIter titbegin = tmrqueue_.begin();
                TMRQIter titend = tmrqueue_.end();
                TMRQIter tit = tmrqueue_.begin();

                Timer now;
                now.setToNow();

                for ( ; tit != titend && tit->first <= now; ++tit)
                {
                    Timer tmr = tit->second.first;
                    EndPoint ep = tit->second.second;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                }

                tmrqueue_.erase(titbegin, tit);
#else
                // we have a timeout. check queue
                Timer now;
                now.setToNow();
                
		while (!tmrqueue_.isEmpty())
		{
                    Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                    tmrqueue_.dequeue(qtimer);

                    if (qtimer.key > now)
                    {
                        tmrqueue_.enqueue(qtimer);
                        break;
                    }

                    Timer tmr = qtimer.data.key;
                    EndPoint ep = qtimer.data.data;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                }
#endif
            }
        }
        else
        {
            HSIter hit = handlers_.begin();
            HSIter hitend = handlers_.end();
            for ( ; hit != hitend; ++hit)
            {
                // skip if not ready
                int socket = hit->first;
                assert(!FD_ISSET(socket, &exceptfds));
                if (!FD_ISSET(socket, &readfds)) continue;
                // get handler
                EndPoint ep = hit->second.first;
                Handler *handler = hit->second.second;

                int status = (*handler)(ep);
                if (status > 0)
                {
                    // success and done with this server
                    FD_CLR(socket, &master_readfds);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
                else if (status < 0)
                {
                    // some type of error. for now, close endpoint
                    if (errno == ECONNRESET)
                    {
                        // other side closed the connection.
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                    else
                    {
                        TMSG("i/o handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                }
            }
        }
    }

    return(0);
}

int
MultiProcMSStreamServer::SubServer::epollLoop()
{
    CheckReturn(ok_);
    CheckReturn(!endpoints_.empty());

    epoll_event ev;
    ::memset(&ev,0,sizeof(ev));

    epoll_event events[DefaultEPollEventsToHandle];

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // do we have any timers?
        int timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
        }
        else
        {
            timer = -1;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
	}
	else
	{
            timer = -1;
	}
#endif

        int eventnum = ::epoll_wait(epollfd_, events, 
                                    DefaultEPollEventsToHandle, timer);
        if (eventnum < 0)
        {
            if (errno == EINTR) continue;
            TMSG("epoll_wait failed");
            DUMP(errno);
            return(-1);
        }
        else if (eventnum == 0)
        {
            // we have a timeout. check queue
#if USESTL
            TMRQIter titbegin = tmrqueue_.begin();
            TMRQIter titend = tmrqueue_.end();
            TMRQIter tit = tmrqueue_.begin();

            Timer now;
            now.setToNow();

            for ( ; tit != titend && tit->first <= now; ++tit)
            {
                Timer tmr = tit->second.first;
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
#else
            // we have a timeout. check queue
            Timer now;
            now.setToNow();
            
            while (!tmrqueue_.isEmpty())
            {
                Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                tmrqueue_.dequeue(qtimer);

                if (qtimer.key > now)
                {
                    tmrqueue_.enqueue(qtimer);
                    break;
                }

                Timer tmr = qtimer.data.key;
                EndPoint ep = qtimer.data.data;

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
#endif
        }
        else
        {
            for (int ievent=0; ievent<eventnum; ++ievent)
            {
                // socket with an event
                int socket = events[ievent].data.fd;

                // get handler
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
                    if (errno == ECONNRESET)
                    {
                        // other side closed the connection.
                        ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                    else
                    {
                        TMSG("i/o handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                }
            }
        }
    }

    return(0);
}

int
MultiProcMSStreamServer::SubServer::finish()
{
    CheckReturn(ok_);

    if (mode_ == EPoll)
    {
        ::close(epollfd_);
        epollfd_ = -1;
    }

    EPIter epit = endpoints_.begin();
    EPIter epitend = endpoints_.end();
    for ( ; epit != epitend; ++epit)
    {
         EndPoint serverep = epit->second;
         serverep.close();
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;

    return(0);
}

// ctors and dtor
MultiProcMSStreamServer::MultiProcMSStreamServer(MultiplexMode mode):
        Server(), mode_(mode), epollfd_(-1), epollsize_(DefaultEPollSize), handlers_(), timerhandlers_(), tmrqueue_()
{
    ok_ = true;
}

MultiProcMSStreamServer::~MultiProcMSStreamServer()
{
    if (ok_)
    {
        EPIter epit = endpoints_.begin();
        EPIter epitend = endpoints_.end();
        for ( ; epit != epitend; ++epit)
        {
             EndPoint serverep = epit->second;
             serverep.close();
        }
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;
}

// register and unregister handlers
int
MultiProcMSStreamServer::registerEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.insert(std::pair<int, EndPoint>(ep.getSocket(), ep));

    return(0);
}

int
MultiProcMSStreamServer::registerHandler(EndPoint &ep, Handler &h)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    h.setServer(this);
    handlers_.insert(std::pair<int, std::pair<EndPoint, Handler * > >(
        ep.getSocket(), std::pair<EndPoint, Handler * >(ep, &h)));

    return(0);
}

int
MultiProcMSStreamServer::registerTimerHandler(EndPoint &ep, TimerHandler &th)
{
    CheckReturn(ok_);

    registerEndPoint(ep);
    th.setServer(this);
    timerhandlers_.insert(std::pair<int, std::pair<EndPoint, TimerHandler * > >(
        ep.getSocket(), std::pair<EndPoint, TimerHandler * >(ep, &th)));

    return(0);
}

int 
MultiProcMSStreamServer::unregisterEndPoint(EndPoint &ep)
{
    CheckReturn(ok_);

    endpoints_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSStreamServer::unregisterHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    handlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSStreamServer::unregisterTimerHandler(EndPoint &ep)
{
    CheckReturn(ok_);

    timerhandlers_.erase(ep.getSocket());

    return(0);
}

int 
MultiProcMSStreamServer::unregisterAllHandlers(EndPoint &ep)
{
    CheckReturn(ok_);

    unregisterEndPoint(ep);
    unregisterHandler(ep);
    unregisterTimerHandler(ep);

    return(0);
}

// set a timer for an end point
void 
MultiProcMSStreamServer::scheduleTimer(EndPoint &myep, Timer &timer)
{
    Timer qtmr(timer);
    Timer now;
    now.setToNow();
    qtmr += now;
#if USESTL
    tmrqueue_.insert(std::pair<Timer, std::pair<Timer, EndPoint> >(qtmr, std::pair<Timer, EndPoint>(timer, myep)));
#else
    tmrqueue_.enqueue(Tuple<Timer, Tuple<Timer, EndPoint> >(qtmr, Tuple<Timer, EndPoint>(timer, myep)));
#endif
}

void 
MultiProcMSStreamServer::cancelTimer(EndPoint &myep, Timer &timer)
{
#if 0
    TMRQIter tmrit = tmrqueue_.find(myep);
    TMRQIter tmritend = tmrqueue_.end();

    if (tmrit == tmritend) return;

    for ( ; tmrit != tmritend; ++tmrit)
    {
        if (tmrit->second == myep) tmrqueue_.erase(tmrit);
    }
#endif
}

// operations
int
MultiProcMSStreamServer::init()
{
    CheckReturn(ok_);

    if (mode_ == EPoll)
    {
        CheckReturn(epollsize_ >= 0);
        epollfd_ = ::epoll_create(epollsize_);
        CheckReturn(epollfd_ >= 0);
    }

    HSIter hit = handlers_.begin();
    HSIter hitend = handlers_.end();
    for ( ; hit != hitend; ++hit)
    {
        EndPoint serverep = hit->second.first;
        CheckReturn(serverep.setReuseAddr(true) == 0);
        CheckReturn(serverep.setNonBlocking(true) == 0);
        CheckReturn(serverep.bind() == 0);
        CheckReturn(serverep.listen() == 0);

        if (mode_ == EPoll)
        {
            serverep.setNonBlocking(true);
            epoll_event ev;
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = serverep.getSocket();
            CheckReturn(::epoll_ctl(epollfd_, EPOLL_CTL_ADD, 
                                    serverep.getSocket(), &ev) == 0);
        }
    }

    return(0);
}

int
MultiProcMSStreamServer::run()
{
    CheckReturn(ok_);

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
MultiProcMSStreamServer::selectLoop()
{
    CheckReturn(ok_);
    CheckReturn(!endpoints_.empty());

    (void) signal(SIGCHLD, reaper);

    fd_set readfds;
    FD_ZERO(&readfds);
    fd_set exceptfds;
    FD_ZERO(&exceptfds);
    fd_set master_readfds;
    FD_ZERO(&master_readfds);

    int maxsocket = -1;
    HSIter hit = handlers_.begin();
    HSIter hitend = handlers_.end();
    for ( ; hit != hitend; ++hit)
    {
        int socket = hit->first;
        if (socket > maxsocket) maxsocket = socket;
        FD_SET(socket, &master_readfds);
    }

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        memcpy(&readfds, &master_readfds, sizeof(master_readfds));
        memcpy(&exceptfds, &master_readfds, sizeof(master_readfds));

        // do we have any timers?
        timeval *ptimer, timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
        }
        else
        {
            ptimer = NULL;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            t2.setTimeval(timer);
            ptimer = &timer;
	}
	else
	{
            ptimer = NULL;
	}
#endif

        int status = ::select(maxsocket+1, &readfds, NULL, &exceptfds, ptimer);
        if (status < 0)
        {
            if (errno == EINTR) continue;
            TMSG("select failed");
            DUMP(errno);
            return(-1);
        }
        else if (status == 0)
        {
            // did timer fire?
            if (ptimer == &timer)
            {
                // we have a timeout. check queue
#if USESTL
                TMRQIter titbegin = tmrqueue_.begin();
                TMRQIter titend = tmrqueue_.end();
                TMRQIter tit = tmrqueue_.begin();

                Timer now;
                now.setToNow();

                for ( ; tit != titend && tit->first <= now; ++tit)
                {
                    Timer tmr = tit->second.first;
                    EndPoint ep = tit->second.second;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
                        ep.close();
                    }
                }

                tmrqueue_.erase(titbegin, tit);
#else
                // we have a timeout. check queue
                Timer now;
                now.setToNow();
                
		while (!tmrqueue_.isEmpty())
		{
                    Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                    tmrqueue_.dequeue(qtimer);

                    if (qtimer.key > now)
                    {
                        tmrqueue_.enqueue(qtimer);
                        break;
                    }

                    Timer tmr = qtimer.data.key;
                    EndPoint ep = qtimer.data.data;

                    int socket = ep.getSocket();
                    THS::iterator hit = timerhandlers_.find(socket);
                    if (hit == timerhandlers_.end()) continue;
                    TimerHandler *handler = hit->second.second;

                    int status = (*handler)(ep, tmr);
                    if (status > 0)
                    {
                        // success and done with this server
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                    else if (status < 0)
                    {
                        // some type of error. for now, close endpoint
                        TMSG("timer handler failed");
                        DUMP(ep); DUMP(socket); DUMP(errno);
                        FD_CLR(socket, &master_readfds);
                        unregisterAllHandlers(ep);
	                ep.close();
                    }
                }
#endif
            }
        }
        else
        {
            HSIter hit = handlers_.begin();
            HSIter hitend = handlers_.end();
            for ( ; hit != hitend; ++hit)
            {
                // skip if not ready
                int socket = hit->first;
                assert(!FD_ISSET(socket, &exceptfds));
                if (!FD_ISSET(socket, &readfds)) continue;
                // get handler
                EndPoint ep = hit->second.first;
                Handler *handler = hit->second.second;
                // accept connection and start up handler
                bool retry;
                UseCntPtr<EndPoint> peer(ep.accept(retry));
                if (peer.ptr() == NULL) continue;
                int peer_socket = peer->getSocket();

		int pid = ::fork();
		if (pid == 0)
		{
                    // the child -

                    // close all sockets not the child
                    EPIter epit = endpoints_.begin();
                    EPIter epitend = endpoints_.end();
                    for ( ; epit != epitend; ++epit)
                    {
                        if (epit->first == peer_socket) continue;
                        EndPoint tmpep = epit->second;
                        unregisterAllHandlers(tmpep);
                        tmpep.close();
                    }

                    // create subserver and run it.
                    SubServer child(mode_);
                    child.registerEndPoint(*peer);
                    child.registerHandler(*peer, *handler);

                    // run the server
                    if (child.init() != 0) exit(-1);
                    if (child.run() != 0) exit(-1);
                    if (child.finish() != 0) exit(-1);

                    // all done
                    exit(0);
		}
                else if (pid > 0)
                {
                    // parent -- close child socket
                    peer->close();
                }
		else if (pid < 0)
		{
                    // error in parent
                    return(-1);
		}
            }
        }
    }

    return(0);
}

int
MultiProcMSStreamServer::epollLoop()
{
    CheckReturn(ok_);
    CheckReturn(!endpoints_.empty());

    (void) signal(SIGCHLD, reaper);

    epoll_event ev;
    memset(&ev,0,sizeof(ev));

    epoll_event events[DefaultEPollEventsToHandle];

    for (Timer zero(0,0); !endpoints_.empty(); )
    {
        // do we have any timers?
        int timer;
#if USESTL
        TMRQIter tit = tmrqueue_.begin();
        if (tit != tmrqueue_.end())
        {
            Timer t2 = tit->first;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
        }
        else
        {
            timer = -1;
        }
#else
        if (!tmrqueue_.isEmpty())
        {
            Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
            tmrqueue_.front(qtimer);
            Timer t2 = qtimer.key;
            Timer now;
            now.setToNow();
            t2 -= now;
            if (t2 < zero) t2 = zero;
            timer = t2.getTimeInMillisecs();
	}
	else
	{
            timer = -1;
	}
#endif

        int eventnum = ::epoll_wait(epollfd_, events, 
                                    DefaultEPollEventsToHandle, timer);
        if (eventnum < 0)
        {
            if (errno == EINTR) continue;
            TMSG("epoll_wait failed");
            DUMP(errno);
            return(-1);
        }
        else if (eventnum == 0)
        {
            // we have a timeout. check queue
#if USESTL
            TMRQIter titbegin = tmrqueue_.begin();
            TMRQIter titend = tmrqueue_.end();
            TMRQIter tit = tmrqueue_.begin();

            Timer now;
            now.setToNow();

            for ( ; tit != titend && tit->first <= now; ++tit)
            {
                Timer tmr = tit->second.first;
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
                    servers.erase(socket);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
                else if (status < 0)
                {
                    // some type of error. for now, close endpoint
                    TMSG("timer handler failed");
                    DUMP(ep); DUMP(socket); DUMP(errno);
                    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket, &ev);
                    servers.erase(socket);
                    unregisterAllHandlers(ep);
                    ep.close();
                }
            }

            tmrqueue_.erase(titbegin, tit);
#else
            // we have a timeout. check queue
            Timer now;
            now.setToNow();
            
            while (!tmrqueue_.isEmpty())
            {
                Tuple<Timer, Tuple<Timer, EndPoint> > qtimer;
                tmrqueue_.dequeue(qtimer);

                if (qtimer.key > now)
                {
                    tmrqueue_.enqueue(qtimer);
                    break;
                }

                Timer tmr = qtimer.data.key;
                EndPoint ep = qtimer.data.data;

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
#endif
        }
        else
        {
            for (int ievent=0; ievent<eventnum; ++ievent)
            {
                // socket with an event
                int socket = events[ievent].data.fd;

                // get handler
                HS::iterator hit = handlers_.find(socket);
                if (hit == handlers_.end()) continue;
                EndPoint ep = hit->second.first;
                Handler *handler = hit->second.second;

                // do an accept ans start up a client
                bool retry;
                UseCntPtr<EndPoint> peer(ep.accept(retry));
                if (peer.ptr() == NULL) continue;
                peer->setNonBlocking(true);
                int peer_socket = peer->getSocket();

		int pid = ::fork();
		if (pid == 0)
		{
                    // the child -

                    // close all sockets not the child
                    EPIter epit = endpoints_.begin();
                    EPIter epitend = endpoints_.end();
                    for ( ; epit != epitend; ++epit)
                    {
                        if (epit->first == peer_socket) continue;
                        EndPoint tmpep = epit->second;
                        unregisterAllHandlers(tmpep);
                        tmpep.close();
                    }

                    // create subserver and run it.
                    SubServer child(mode_);
                    child.registerEndPoint(*peer);
                    child.registerHandler(*peer, *handler);

                    // run the server
                    if (child.init() != 0) exit(-1);
                    if (child.run() != 0) exit(-1);
                    if (child.finish() != 0) exit(-1);

                    // all done
                    exit(0);
		}
                else if (pid > 0)
                {
                    // parent -- close child socket
                    peer->close();
                }
		else if (pid < 0)
		{
                    // error in parent
                    return(-1);
		}
            }
        }
    }

    return(0);
}

int
MultiProcMSStreamServer::finish()
{
    CheckReturn(ok_);

    if (mode_ == EPoll)
    {
        ::close(epollfd_);
        epollfd_ = -1;
    }

    EPIter epit = endpoints_.begin();
    EPIter epitend = endpoints_.end();
    for ( ; epit != epitend; ++epit)
    {
         EndPoint serverep = epit->second;
         serverep.close();
    }

    endpoints_.clear();
    handlers_.clear();
    timerhandlers_.clear();

    ok_ = false;

    return(0);
}

}
