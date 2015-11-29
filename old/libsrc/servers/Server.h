#ifndef __OMBT_SERVER_H
#define __OMBT_SERVER_H

// local headers
#include <map>
#include <atomic/BaseObject.h>
#include <sockets/EndPoint.h>
#include <timers/Timer.h>

namespace ombt {

// server for a connection
class Server: public BaseObject
{
public:
    // reactor type
    enum MultiplexMode { None, Select, EPoll, Poll };
    enum { DefaultEPollSize = 128 };
    enum { DefaultEPollEventsToHandle = 128 };

    // ctors and dtor
    Server(): BaseObject(true) { }
    Server(const Server &src): BaseObject(src) { }
    virtual ~Server() { };

    // assignment
    Server &operator=(const Server &rhs) {
        if (this != &rhs) {
            BaseObject::operator=(rhs);
        }
        return(*this);
    }

    // operations
    virtual int init() { return(-1); }
    virtual int run() { return(-1); }
    virtual int finish() { return(-1); }

    // set a timer for an end point
    virtual void scheduleTimer(EndPoint &, Timer &) { }
    virtual void cancelTimer(EndPoint &, Timer &) { }

protected:
    bool done_;
};

}

#endif
