#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <list>
#include <vector>

#include <Debug.h>
#include <logging/Logging.h>

#include <threads/ThreadBarrier.h>
#include <threads/ThreadMutex.h>
#include <frameworks/Job.h>
#include <frameworks/Queue.h>
#include <frameworks/ThreadPool.h>

using namespace ombt;
using namespace std;

class MyJob: public Job {
public:
    MyJob(int maxrestarts, ThreadBarrier &barrier):
        restarts_(0), maxrestarts_(maxrestarts), barrier_(barrier)
    {
DBGTRACE();
        MustBeTrue(maxrestarts_ >= 0);
    }
    virtual ~MyJob()
    {
DBGTRACE();
dbgprintf("MyJob dtor: restartd = %d, maxrestarts = %d\n", restarts_, maxrestarts_);
    }
    
    virtual Job::JobReturn run()
    {
//DBGTRACE();
        ThreadMutexLock lock(mutex_);
        if (++restarts_ <= maxrestarts_)
        {
//DBGTRACE();
            return Job::Blocked;
        }
        else
        {
DBGTRACE();
            barrier_.unblock();
            return Job::Finished;
        }
    } 

private:
    int restarts_;
    int maxrestarts_;
    ThreadMutex mutex_;
    ThreadBarrier &barrier_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 4)
    {
        cerr << "uasge: " << argv[0] << " maxrestarts maxjobs maxthreads" << endl;
        return(2);
    }

    int arg = 0;
    int maxrestarts = atoi(argv[++arg]);
    int maxjobs = atoi(argv[++arg]);
    int maxthreads = atoi(argv[++arg]);

DBGPRINTF("%s", "before barrier ctors");
    vector<UseCntPtr<ThreadBarrier> > barriers;
    for (int j=0; j<maxjobs; ++j)
    {
DBGPRINTF("before barrier ctor %d:", j);
        UseCntPtr<ThreadBarrier> ptb(new ThreadBarrier);
DBGPRINTF("after barrier ctor %d:", j);
DBGPRINTF("before barrier block %d:", j);
        ptb->block();
DBGPRINTF("after barrier block %d:", j);
        barriers.push_back(ptb);
    }
DBGPRINTF("%s", "after barrier ctors");

DBGPRINTF("before pool ctor, threads = %d", maxthreads);
    ThreadPool pool(maxthreads);
DBGPRINTF("after pool ctor, threads = %d", maxthreads);

DBGPRINTF("before addJob and MyJob ctor, jobs = %d", maxjobs);
    for (int j=0; j<maxjobs; ++j)
    {
DBGPRINTF("before addJob and MyJob ctor, job = %d", j);
        pool.addJob(new MyJob(maxrestarts, *barriers[j]));
DBGPRINTF("after addJob and MyJob ctor, job = %d", j);
    }
DBGPRINTF("%s", "after addJob and MyJob ctor");

DBGPRINTF("%s", "before all barrier wait");
    vector<UseCntPtr<ThreadBarrier> >::iterator tbi = barriers.begin();
    vector<UseCntPtr<ThreadBarrier> >::iterator tbiend = barriers.end();
    for ( ; tbi != tbiend; ++tbi)
    {
        (*tbi)->wait();
    }
DBGPRINTF("%s", "after all barrier wait");

    return(0);
}
