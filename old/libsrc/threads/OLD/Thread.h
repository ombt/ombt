#ifndef __OMBT_THREAD_H
#define __OMBT_THREAD_H

// headers
#include <pthread.h>
#include "BaseObject.h"

namespace ombt {

// thread class
class Thread: public BaseObject {
public:
    Thread();
    Thread(const Thread &src);
    virtual ~Thread();

    // assignment
    Thread &operator=(const Thread &rhs);

    // maintenance
    virtual int run();
    virtual int cancel();
    virtual int join();
    virtual int detach();

    // get attributes
    inline bool isDetached() const;
    inline bool isStarted() const { return(id_ != -1); }

    // the actual function to run
    virtual void *entry();

protected:
    // start up routine
    static void *start_routine(void *arg);

protected:
    // internal data
    struct Data {
        pthread_t id_;
        pthread_attr_t attr_;
    } *data_;
};

}

#endif
