#ifndef __OMBT_THREAD_H
#define __OMBT_THREAD_H
// thread class

// headers
#include <pthread.h>
#include <Debug.h>
#include <atomic/BaseObject.h>
#include <atomic/UseCntPtr.h>
#include <threads/ThreadMutex.h>
#include <frameworks/Task.h>

namespace ombt {

// thread class
class Thread: public BaseObject {
public:
    // thread entry point
    static void *execute(void *data);
    void *dispatch();

public:
    // ctor and dtor
    Thread(Task *ptask);
    ~Thread();

    // maintenance
    int run();
    int join(void *&retval);
    int detach();
    int cancel();

    // set attributes
    void setDetachedAttr(bool flag);

    // get attributes
    bool isStarted() const;
    bool isDetached() const;
    bool isCanceled() const;
    bool isJoined() const;
    bool isJoinable() const;
    unsigned long getHandle() const;

private:
    static unsigned long nextHandle();

private:
    // leper colony
    Thread();
    Thread(const Thread &src);
    Thread &operator=(const Thread &rhs);

protected:
    // static internal data
    unsigned long handle_;
    UseCntPtr<Task> ptask_;

    // dynamic internal data
    struct State {
        ThreadMutex cs_;
        pthread_t id_;
        pthread_attr_t attr_;
        bool canceled_;
        bool joined_;
        bool started_;

        State(): 
            cs_(), canceled_(false), joined_(false), started_(false) { }
    } state_;
};

}

#endif
