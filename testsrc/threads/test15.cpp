//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <list>

#include <Debug.h>
#include <logging/Logging.h>

#include <threads/ThreadBarrier.h>
#include <threads/ThreadMutex.h>
#include <threads/Job.h>
#include <threads/Queue.h>
#include <threads/ThreadPool.h>

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
DBGTRACE();
        ThreadMutexLock lock(mutex_);
        if (++restarts_ <= maxrestarts_)
        {
DBGTRACE();
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

    if (argc != 2)
    {
        cerr << "uasge: " << argv[0] << " maxrestarts" << endl;
        return(2);
    }

    int arg = 0;
    int maxrestarts = atoi(argv[++arg]);

DBGPRINTF("%s", "before barrier ctor");
    ThreadBarrier barrier;
DBGPRINTF("%s", "after barrier ctor");

DBGPRINTF("%s", "before barrier block");
    barrier.block();
DBGPRINTF("%s", "after barrier block");

DBGPRINTF("%s", "before pool ctor");
    ThreadPool pool(4);
DBGPRINTF("%s", "after pool ctor");

DBGPRINTF("%s", "before addJob and MyJob ctor");
    pool.addJob(new MyJob(maxrestarts, barrier));
DBGPRINTF("%s", "after addJob and MyJob ctor");

DBGPRINTF("%s", "before barrier wait");
    barrier.wait();
DBGPRINTF("%s", "after barrier wait");

    return(0);
}
