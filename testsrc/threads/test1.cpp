//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <iostream>
#include "threads/ThreadMutex.h"

using namespace ombt;

main(int argc, char *argv[])
{
    BaseMutex *bm = new NullMutex;
    bm->lock();
    bm->unlock();

    BaseMutex *bm2 = new ThreadMutex;
    bm2->lock();
    bm2->unlock();

    {
        ThreadMutexLock lock(*bm);
    }

    {
        ThreadMutexLock lock(*bm2);
    }

    delete bm;
    delete bm2;

    return(0);
}
