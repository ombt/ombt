#ifndef __OMBT_THREAD_MUTEX_H
#define __OMBT_THREAD_MUTEX_H
// mutex class

// headers
#include <pthread.h>
#include <Debug.h>

// thread mutex class
class BaseMutex {
public:
    // ctor and dtor
    BaseMutex() { }
    virtual ~BaseMutex() { }

    // enable or disable on demand
    virtual void lock() = 0;
    virtual void unlock() = 0;
    
private:
    // not allowed
    BaseMutex(const BaseMutex &src);
    BaseMutex &operator=(const BaseMutex &rhs);
};

class NullMutex: public BaseMutex {
public:
    // ctor and dtor
    NullMutex(): BaseMutex() { }
    ~NullMutex() { }

    // do nothing lock and unlock
    inline void lock() { }
    inline void unlock() { }
    
private:
    // not allowed
    NullMutex(const NullMutex &src);
    NullMutex &operator=(const NullMutex &rhs);
};

// thread mutex class
class ThreadMutex: public BaseMutex {
public:
    // ctor and dtor
    ThreadMutex(): BaseMutex()  {
         pthread_mutex_init(&mutex_, 0);
    }
    ~ThreadMutex() {
         pthread_mutex_destroy(&mutex_);
    }

    // enable or disable on demand
    inline void lock() {
         pthread_mutex_lock(&mutex_);
    }
    inline void unlock() {
         pthread_mutex_unlock(&mutex_);
    }
    
private:
    // not allowed
    ThreadMutex(const ThreadMutex &src);
    ThreadMutex &operator=(const ThreadMutex &rhs);

protected:
    pthread_mutex_t mutex_;
};

class ThreadMutexLock {
public:
    // ctor and dtor
    ThreadMutexLock(BaseMutex &mutex):
                    mutex_(mutex) {
         mutex_.lock();
    }
    ~ThreadMutexLock() {
         mutex_.unlock();
    }

private:
    // not allowed
    ThreadMutexLock(const ThreadMutexLock &src);
    ThreadMutexLock &operator=(const ThreadMutexLock &rhs);

protected:
    BaseMutex &mutex_;
};

#endif
