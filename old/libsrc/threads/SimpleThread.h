#ifndef __OMBT_SIMPLE_THREAD_H
#define __OMBT_SIMPLE_THREAD_H
// thread class

// headers
#include <pthread.h>
#include <atomic/BaseObject.h>
#include <atomic/UseCntPtr.h>

namespace ombt {

// simple thread class
class SimpleThread: public BaseObject {
public:
    // base class for all runnable classes
    class Task: public BaseObject {
    public:
        // ctor and dtor
        Task() { }
        Task(const Task &src) { }
        virtual ~Task() { }

        Task &operator=(const Task &rhs) { return(*this); }

        virtual int init() { return 0; }
        virtual int run() { return 0; }
        virtual int finish() { return 0; }
    };

    // thread entry point
    static void *execute(void *data);
    void *dispatch();

public:
    // ctor and dtor
    SimpleThread(Task *runnable);
    ~SimpleThread();

    // maintenance
    int run();
    int join(void *&retval);
    int detach();
    int cancel();

    // set attributes
    void setDetachedAttr(bool flag);

    // get attributes
    inline bool isJoinable() const;
    inline bool isDetached() const;
    inline bool isStarted() const { return(id_ != -1); }

private:
    // leper colony
    SimpleThread();
    SimpleThread(const SimpleThread &src);
    SimpleThread &operator=(const SimpleThread &rhs);

protected:
    // internal data
    UseCntPtr<Task> ptask_;
    pthread_t id_;
    pthread_attr_t attr_;
};

}

#endif
