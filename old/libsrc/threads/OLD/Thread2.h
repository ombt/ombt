#ifndef __OMBT_THREAD_H
#define __OMBT_THREAD_H
// thread class

// headers
#include <pthread.h>
#include "BaseObject.h"
#include "RefCntPtr.h"
#include "ThreadMutex.h"

namespace ombt {

// thread class
template <class MutexType>
class Thread: public BaseObject {
public:
    // base class for all runnable classes
    class Runnable {
    public:
        // ctor and dtor
        Runnable() { }
        Runnable(const Runnable &src) { }
        virtual ~Runnable() { }

        Runnable &operator=(const Runnable &rhs) { return(*this); }

        virtual void *operator()() { return(NULL); }
    };

public:
    Thread(Runnable *runnable);
    Thread(const Thread &src);
    ~Thread();

    // assignment
    Thread &operator=(const Thread &rhs);

    // maintenance
    int run();
    int cancel();
    int join();
    int detach();

    // get attributes
    inline bool isDetached() const;
    inline bool isStarted() const { return(id_ != -1); }

protected:
    // start up routine
    static void *start_routine(void *arg);

protected:
    // internal data
    struct Data {
        Runnable *runnable_;
        pthread_t id_;
        pthread_attr_t attr_;
    };

    RefCntPtr<Data, MutexType> data_;
};

}

#endif
