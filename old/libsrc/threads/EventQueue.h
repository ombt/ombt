#ifndef __OMBT_EVENT_QUEUE_H
#define __OMBT_EVENT_QUEUE_H

// threadpool event queue

// headers
#include <list>
#include <limits>
#include <pthread.h>
#include <Debug.h>

namespace ombt {

// generic event queue class
template <class EventType>
class EventQueue {
public:
    // ctor and dtor
    EventQueue(int maxsize = std::numeric_limits<int>::max()): 
        done_(false), maxsize_(maxsize), events_() {
        MustBeTrue(maxsize_ >= 1);
        pthread_cond_init(&cv_, NULL);
        pthread_mutex_init(&cv_mutex_, NULL);
    }
    ~EventQueue() {
        pthread_cond_destroy(&cv_);
        pthread_mutex_destroy(&cv_mutex_);
    }

    // accessing the queue
    void enqueue(EventType &event) {
        pthread_mutex_lock(&cv_mutex_);
        if (events_.size() <= maxsize_)
            events_.push_back(event);
        pthread_cond_signal(&cv_);
        pthread_mutex_unlock(&cv_mutex_);
    }
    bool dequeue(EventType &event) {
        pthread_mutex_lock(&cv_mutex_);
        while (events_.empty())
        {
            if (done_)
            {
                pthread_cond_signal(&cv_);
                pthread_mutex_unlock(&cv_mutex_);
                return false;
            }
            pthread_cond_wait(&cv_, &cv_mutex_);
        }
        event = events_.front();
        events_.pop_front();
        pthread_cond_signal(&cv_);
        pthread_mutex_unlock(&cv_mutex_);
        return true;
    }
    void done() {
        pthread_mutex_lock(&cv_mutex_);
        done_ = true;
        pthread_cond_broadcast(&cv_);
        pthread_mutex_unlock(&cv_mutex_);
    }

protected:
    // data
    typedef std::list<EventType> Queue;

    bool done_;
    int maxsize_;
    Queue events_;
    pthread_cond_t cv_;
    pthread_mutex_t cv_mutex_;
};

}

#endif
