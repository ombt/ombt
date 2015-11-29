#ifndef __OMBT_TIMER_QUEUE_H
#define __OMBT_TIMER_QUEUE_H
// queue for timers 

// headers
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <atomic/BaseObject.h>
#include <timers/Timer.h>
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>

namespace ombt {

// queue for generic timer
class TimerQueue: public BaseObject
{
public:
    // ctor and dtor
    TimerQueue();
    ~TimerQueue();

    // accessor functions to timer priority queue
    int isEmpty() const { return(pqueue_.empty()); }
    int enqueue(const Timer &timer);
    int dequeue(Timer &timer);
    int peek(Timer &timer) const;
    const Timer peek() const;

    // time to next timer
    timeval *getTimeOut(timeval *tv);

private:
    // not possible with stl pqueue
    TimerQueue(const TimerQueue &src);
    TimerQueue &operator=(const TimerQueue &rhs);

protected:
    // we want the queue to have times in increasing order
    std::priority_queue<Timer, std::vector<Timer>, std::greater<Timer> > pqueue_;
};

}

#endif
