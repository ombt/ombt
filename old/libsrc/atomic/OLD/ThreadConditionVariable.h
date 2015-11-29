#ifndef __OMBT_THREAD_COND_VAR_H
#define __OMBT_THREAD_COND_VAR_H

// thread condition variable

// headers
#include <pthread.h>

namespace ombt {

// thread condition variable class
class ThreadConditionVariable {
public:
    // ctor and dtor
    ThreadConditionVariable(): 
        blocked_(false) {
        pthread_cond_init(&cv_, NULL);
        pthread_mutex_init(&cv_mutex_, NULL);
    }
    ~ThreadConditionVariable() {
        blocked_ = false;
        pthread_cond_destroy(&cv_);
        pthread_mutex_destroy(&cv_mutex_);
    }

    // signal done 
    void signal() {
        pthread_mutex_lock(&cv_mutex_);
        blocked_ = false;
        pthread_cond_broadcast(&cv_);
        pthread_mutex_unlock(&cv_mutex_);
    }

    // wait for signal
    void wait() {
        // wait for barrier to go down, then proceed
        pthread_mutex_lock(&cv_mutex_);
        while (blocked_)
        {
            pthread_cond_wait(&cv_, &cv_mutex_);
            // DBGTRACE();
        }
        // allow all other threads to proceed
        blocked_ = false;
        pthread_mutex_unlock(&cv_mutex_);
    }

private:
    // leper colony
    ThreadConditionVariable(const ThreadConditionVariable &src);
    ThreadConditionVariable &operator=(const ThreadConditionVariable &rhs);

private:
    pthread_mutex_t cv_mutex_;
    pthread_cond_t cv_;
    bool blocked_;
};

}

#endif
