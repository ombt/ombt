#ifndef __OMBT_TASK_H
#define __OMBT_TASK_H
// task class

// headers
#include <pthread.h>
#include <Debug.h>
#include <atomic/BaseObject.h>

namespace ombt {

// base class for all runnable classes
class Task: public BaseObject {
public:
    // ctor and dtor
    Task();
    Task(const Task &src);
    virtual ~Task();

    Task &operator=(const Task &rhs);
    virtual Task *clone() const;

    virtual int init();
    virtual int run();
    virtual int finish();
};

}

#endif
