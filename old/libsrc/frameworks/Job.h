#ifndef __OMBT_JOB_H
#define __OMBT_JOB_H
// job class

// headers
#include <pthread.h>
#include <Debug.h>
#include <atomic/BaseObject.h>

namespace ombt {

// base class for all runnable classes
class Job: public BaseObject {
public:
    enum JobReturn { Finished, Error, Blocked };

    // ctor and dtor
    Job();
    virtual ~Job();

    // service
    virtual JobReturn run();
    unsigned long getJobId() const;

private:
    static unsigned long nextJobId();

private:
    // leper colony
    Job(const Job &src);
    Job &operator=(const Job &rhs);

protected:
    unsigned long jobId_;
};

}

#endif
