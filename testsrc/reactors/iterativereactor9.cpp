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
#include "files/Notify.h"

using namespace ombt;
using namespace std;

// timer handler
class MyTimerHandler: public IterativeReactor::BaseTimerHandler {
public:
    MyTimerHandler(): IterativeReactor::BaseTimerHandler() { }
    virtual ~MyTimerHandler() { }

    virtual int handler(EndPoint *psep, Timer mytimer) {
        dbgprintf("In Timer Handler !!!\n");
        pserver_->finish();
        return(0);
    }
};

// read handler
class NotifyHandler: public IterativeReactor::BaseIOHandler {
public:
    NotifyHandler(): IterativeReactor::BaseIOHandler() { }
    virtual ~NotifyHandler() { }

    virtual int handler(EndPoint *pserver_ep) {
        UseCntPtr<EndPoint> pep(pserver_ep);
        dbgprintf("In Echo Handler, socket is %d\n", pep->getSocket());
        unsigned char buf[10*1024];
        size_t count = 1024;
        int status = pep->read(buf, count);
        if (status == 0)
            return(1);
        else if (status < 0)
            return(-1);
        else
        {
            count = status;
            UseCntPtr<NotifyEndPoint> pnep(dynamic_cast<NotifyEndPoint *>(pserver_ep));
            inotify_event *pevent = (inotify_event *)buf;
            for (int offset = 0; offset<count;
                 offset += offset + sizeof(inotify_event) + pevent->len)
            {
                std::string path;
                pnep->getPath(pevent->wd, path);
                dbgprintf("Watch descriptor: %d\n", pevent->wd);
                dbgprintf("Event Path      : %s\n", path.c_str());
                dbgprintf("Event Mask      : %lu\n", pevent->mask);
                dbgprintf("Event Cookie    : %lu\n", pevent->cookie);
                dbgprintf("Event Length    : %lu\n", pevent->len);
                if (pevent->len > 0)
                    dbgprintf("File Name       : %s\n", pevent->name);
            }
        }
    }
};

// server thread task
class ServerTask: public Task {
public:
    ServerTask(int seconds, NotifyEndPoint* pnep,
               IterativeReactor::BaseIOHandler *pbioh):
        Task(), seconds_(seconds), pnep_(pnep), pbioh_(pbioh) {
    }
    ~ServerTask() {
    }

    virtual int init() {
        dbgprintf("Create an IterativeReactor ...\n");
        preactor_ = UseCntPtr<IterativeReactor>(new IterativeReactor);
        MustBeTrue(preactor_ != NULL);

        dbgprintf("Register Notifier EndPoint ...\n");
        MustBeTrue(preactor_->registerEndPoint(pnep_) == OK);

        dbgprintf("Register Notifier handler ...\n");
        MustBeTrue(preactor_->registerReadHandler(pnep_, pbioh_) == OK);

        dbgprintf("Create a %d second timer...\n", seconds_);
        Timer tmout(seconds_, 0);

        dbgprintf("Create Timer Handler ...\n");
        UseCntPtr<IterativeReactor::BaseTimerHandler> pbth(new MyTimerHandler);

        dbgprintf("Schedule a timer ...\n");
        MustBeTrue(preactor_->scheduleTimer(pnep_, tmout, pbth) == OK);

        dbgprintf("Run init() ...\n");
        MustBeTrue(preactor_->init() == OK);

        return 0;
    }
    virtual int run() {
        dbgprintf("Run run() ...\n");
        MustBeTrue(preactor_->run() == OK);
        return 0;
    }
    virtual int finish() {
        dbgprintf("Run finish() ...\n");
        MustBeTrue(preactor_->finish() == OK);
        return 0;
    }

private:
    int seconds_;
    UseCntPtr<IterativeReactor> preactor_;
    UseCntPtr<NotifyEndPoint> pnep_;
    UseCntPtr<IterativeReactor::BaseIOHandler> pbioh_;
};

int
main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc < 3)
    {
        dbgprintf("usage: %s seconds dir1 [dir2 ...]\n", argv[0]);
        return(2);
    }

    int arg = 0;
    int seconds = atoi(argv[++arg]);

    dbgprintf("Create a Notifier Socket ...\n");
    UseCntPtr<Notify> pnotifier(new Notify);
    MustBeTrue(pnotifier != NULL);

    dbgprintf("Create a Notifier EndPoint ...\n");
    UseCntPtr<NotifyEndPoint> pnep(new NotifyEndPoint(pnotifier));
    MustBeTrue(pnep != NULL);

    for (++arg; arg<argc; ++arg)
    {
        pnep->watch(argv[arg], IN_CLOSE_WRITE|IN_MODIFY|IN_CREATE|IN_DELETE);
    }

    dbgprintf("Create Notifier Handler ...\n");
    UseCntPtr<IterativeReactor::BaseIOHandler> pbioh(new NotifyHandler);

    UseCntPtr<Thread> pserver(new Thread(new ServerTask(seconds, pnep, pbioh)));
    MustBeTrue(pserver->isOk());
    MustBeTrue(pserver->run() == 0);
    MustBeTrue(pserver->isOk());

    void *retval;
    pserver->join(retval);

    return(0);
}

