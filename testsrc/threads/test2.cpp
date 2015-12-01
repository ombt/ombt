//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdio.h>
#include <iostream>
#include <list>
#include "system/Debug.h"
#include "threads/SimpleThread.h"
#include "logging/Logging.h"

using namespace ombt;
using namespace std;

static bool spin = true;

class Work: public SimpleThread::Task {
public:
    Work() {
DBGTRACE();
    }
    ~Work() {
DBGTRACE();
    }
    virtual int init() {
        while (spin);
DBGTRACE();
        return 0;
    }
    virtual int run() {
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

DBGTRACE();
    for (int i=1; i<=imax; ++i)
    {
DBGTRACE();
        UseCntPtr<SimpleThread> pst(new SimpleThread(new Work));
DBGTRACE();
        MustBeTrue(pst->isOk());
DBGTRACE();

        MustBeTrue(pst->run() == 0);
DBGTRACE();
        MustBeTrue(pst->isOk());
DBGTRACE();

        MustBeTrue(pst->detach() == 0);
DBGTRACE();

        void *retval;
DBGTRACE();
        MustBeTrue(pst->join(retval) != 0);
DBGTRACE();
        MustBeTrue(pst->isOk());
DBGTRACE();
        threads.push_back(pst);
    }
DBGTRACE();

    ::sleep(5);
DBGTRACE();
    spin = false;
DBGTRACE();
    ::sleep(5);
DBGTRACE();
    threads.clear();
DBGTRACE();

    return(0);
}

