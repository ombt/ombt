// thread class

// headers
#include <Debug.h>
#include <Job.h>
#include <logging/Logging.h>
#include <threads/ThreadMutex.h>

namespace ombt {

// generate next id
unsigned long 
Job::nextJobId()
{
    static ThreadMutex mutex;
    static unsigned long nextJobToUse = 0;
    ThreadMutexLock lock(mutex);
    return(++nextJobToUse);
}

// ctor and dtor
Job::Job(): BaseObject(), jobId_(nextJobId())
{
    ok_ = true;
}
Job::~Job()
{
	// do nothing
}

// job services
unsigned long
Job::getJobId() const
{
    return jobId_;
}

Job::JobReturn
Job::run()
{
    ok_ = false;
    return Finished;
}

}
