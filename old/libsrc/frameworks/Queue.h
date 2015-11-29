#ifndef __OMBT_QUEUE_H
#define __OMBT_QUEUE_H

// mutex-protected queue

// headers
#include <list>
#include <limits>
#include <pthread.h>

#include <Debug.h>
#include <logging/Logging.h>

namespace ombt {

// generic queue class
template <class ItemType>
class Queue {
public:
    // ctor and dtor
    Queue(int maxsize = std::numeric_limits<int>::max()): 
        done_(false), maxsize_(maxsize), queue_() {
        MustBeTrue(maxsize_ >= 1);
        pthread_cond_init(&cv_, NULL);
        pthread_mutex_init(&cv_mutex_, NULL);
    }
    ~Queue() {
        pthread_cond_destroy(&cv_);
        pthread_mutex_destroy(&cv_mutex_);
    }

    // accessing the queue
    void enqueue(const ItemType &event) {
        pthread_mutex_lock(&cv_mutex_);
        if (done_)
        {
            pthread_mutex_unlock(&cv_mutex_);
            pthread_cond_signal(&cv_);
            return;
        }
        if (queue_.size() <= maxsize_)
            queue_.push_back(event);
        pthread_mutex_unlock(&cv_mutex_);
        pthread_cond_signal(&cv_);
    }
    bool dequeue(ItemType &event) {
        pthread_mutex_lock(&cv_mutex_);
        while (queue_.empty())
        {
            if (done_)
            {
                pthread_mutex_unlock(&cv_mutex_);
                pthread_cond_signal(&cv_);
                return false;
            }
            pthread_cond_wait(&cv_, &cv_mutex_);
        }
        event = queue_.front();
        queue_.pop_front();
        pthread_mutex_unlock(&cv_mutex_);
        pthread_cond_signal(&cv_);
        return true;
    }
    void done() {
        pthread_mutex_lock(&cv_mutex_);
        done_ = true;
        pthread_mutex_unlock(&cv_mutex_);
        pthread_cond_broadcast(&cv_);
    }

protected:
    // data
    typedef std::list<ItemType> QueueType;

    bool done_;
    int maxsize_;
    QueueType queue_;
    pthread_cond_t cv_;
    pthread_mutex_t cv_mutex_;
};

}

#endif
