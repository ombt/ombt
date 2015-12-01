//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "threads/SimpleThread.h"

#include <stdio.h>
#include <iostream>
#include <list>
#include "system/Debug.h"
#include "threads/SimpleThread.h"
#include "logging/Logging.h"
#include "atomic/UseCntPtr.h"

using namespace ombt;
using namespace std;

static bool spin = true;

class First: public SimpleThread::Task {
public:
    virtual int init() {
        DBGTRACE();
        return 0;
    }
    virtual int run() {
        DBGTRACE();
        sleep(2); 
        spin = false;
        sleep(1);
        DBGTRACE();
        return 0;
    }
    virtual int finish() {
        DBGTRACE();
        return 0;
    }
};

class Second: public SimpleThread::Task {
public:
    virtual int init() {
        DBGTRACE();
        return 0;
    }
    virtual int run() {
        DBGTRACE();
        while (spin);
        DBGTRACE();
        return 0;
    }
    virtual int finish() {
        DBGTRACE();
        return 0;
    }
};
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    MustBeTrue(argc == 2);

    int imax = ::atoi(argv[1]);
    MustBeTrue(imax > 0);

    std::list<UseCntPtr<SimpleThread> > threads;

    for (int i=1; i<=imax; ++i)
    {
        DBGTRACE();
        UseCntPtr<SimpleThread> pst(new SimpleThread(new First));
        MustBeTrue(pst->isOk());

        DBGTRACE();
        MustBeTrue(pst->run() == 0);
        MustBeTrue(pst->isOk());

        DBGTRACE();
        UseCntPtr<SimpleThread> pst2(new SimpleThread(new Second));
        MustBeTrue(pst2->isOk());

        DBGTRACE();
        MustBeTrue(pst2->run() == 0);
        MustBeTrue(pst2->isOk());

        void *retval;

        DBGTRACE();
        pst->join(retval);
        MustBeTrue(!pst->isOk());

        DBGTRACE();
        pst2->join(retval);
        MustBeTrue(!pst2->isOk());

        DBGTRACE();
    }

    return(0);
}

