#ifndef __OMBT_THREAD_BARRIER_H
#define __OMBT_THREAD_BARRIER_H

// one-time barrier - threads block until released

// headers
#include <pthread.h>
#include <atomic/BaseObject.h>
#include <logging/Logging.h>

namespace ombt {

// thread barrier class
class ThreadBarrier: public BaseObject {
public:
    // ctor and dtor
    ThreadBarrier(bool blocked = false): 
        BaseObject(),
        blocked_(blocked) {
        pthread_cond_init(&cv_, NULL);
        pthread_mutex_init(&cv_mutex_, NULL);
    }
    ~ThreadBarrier() {
        pthread_cond_destroy(&cv_);
        pthread_mutex_destroy(&cv_mutex_);
    }

    // block and unblock calls
    void block() {
        pthread_mutex_lock(&cv_mutex_);
        blocked_ = true;
    }
    void unblock() {
        blocked_ = false;
        pthread_cond_broadcast(&cv_);
        pthread_mutex_unlock(&cv_mutex_);
    }

    // wait for unblock
    void wait() {
        // wait for barrier to go down, then proceed
        pthread_mutex_lock(&cv_mutex_);
        while (blocked_)
        {
            pthread_cond_wait(&cv_, &cv_mutex_);
        }
        // allow all other threads to proceed
        blocked_ = false;
        pthread_mutex_unlock(&cv_mutex_);
    }

private:
    // leper colony
    ThreadBarrier(const ThreadBarrier &src);
    ThreadBarrier &operator=(const ThreadBarrier &rhs);

private:
    pthread_mutex_t cv_mutex_;
    pthread_cond_t cv_;
    bool blocked_;
};

}

#endif
