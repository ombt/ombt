//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// test for iterative reactor class

// system headers
#include <string>

// local headers
#include "system/Debug.h"
#include "logging/Logging.h"
#include "atomic/UseCntPtr.h"
#include "timers/Timer.h"
#include "reactors/IterativeReactor.h"
#include "threads/Thread.h"
#include "threads/Task.h"
#include "threads/ThreadBarrier.h"
#include "signals/Signal.h"

using namespace ombt;
using namespace std;

// timer handler
class MyTimerHandler: public IterativeReactor::BaseTimerHandler {
public:
    MyTimerHandler(int howmany, int whentostop): 
        IterativeReactor::BaseTimerHandler(), howmany_(howmany), whentostop_(whentostop) { }
    virtual ~MyTimerHandler() { }

    virtual int handler(EndPoint *pserver_ep, Timer mytimer) {
        howmany_ -= 1;
        dbgprintf("In Timer Handler (how many left =%d) ...\n", howmany_);
        if (mytimer.getType() == Timer::RepeatTimer)
        {
            if (howmany_ > 0)
            {
                if (howmany_ == whentostop_)
                {
                    pserver_->cancelTimer(NULL, mytimer, NULL);
                }
            }
            else
            {
                pserver_->finish();
            }
        }
        else
        {
            pserver_->finish();
        }
        return(0);
    }

    int howmany_;
    int whentostop_;
};

// server thread task
class ServerTask: public Task {
public:
    ServerTask(int seconds, int howmany, int whentostop): 
        Task(), seconds_(seconds), howmany_(howmany), whentostop_(whentostop) { }
    ~ServerTask() { }

    virtual int init() {
        dbgprintf("Server: Create an IterativeReactor ...\n");
        preactor_ = UseCntPtr<IterativeReactor>(new IterativeReactor(IterativeReactor::EPoll));
        MustBeTrue(preactor_ != NULL);

        dbgprintf("Server: Create a %d second timer...\n", seconds_);
        Timer tmout(seconds_, 0);
        if (howmany_ > 1) tmout.setType(Timer::RepeatTimer);

        dbgprintf("Server: Create Timer Handler ...\n");
        UseCntPtr<IterativeReactor::BaseTimerHandler> pbth(new MyTimerHandler(howmany_, whentostop_));

        dbgprintf("Server: Schedule a timer ...\n");
        MustBeTrue(preactor_->scheduleTimer(NULL, tmout, pbth) == OK);

        dbgprintf("Server: Run init() ...\n");
        MustBeTrue(preactor_->init() == OK);

        return 0;
    }
    virtual int run() {
        dbgprintf("Server: Run run() ...\n");
        MustBeTrue(preactor_->run() == OK);
        return 0;
    }
    virtual int finish() {
        dbgprintf("Server: Run finish() ...\n");
        MustBeTrue(preactor_->finish() == OK);
        return 0;
    }

private:
    int seconds_;
    int howmany_;
    int whentostop_;
    UseCntPtr<IterativeReactor> preactor_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 4)
    {
        cerr << "usage: " << argv[0] << " seconds howmany whentostop" << endl;
        return(2);
    }

    int arg = 0;
    int seconds = atoi(argv[++arg]);
    int howmany = atoi(argv[++arg]);
    int whentostop = atoi(argv[++arg]);

    UseCntPtr<Thread> pserver(new Thread(new ServerTask(seconds, howmany, whentostop)));
    MustBeTrue(pserver->isOk());
    MustBeTrue(pserver->run() == 0);
    MustBeTrue(pserver->isOk());

    void *retval;
    pserver->join(retval);

    return(0);
}

