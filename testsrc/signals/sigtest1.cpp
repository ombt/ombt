//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// simple test for signals class

// system headers
#include <string>

// local headers
#include "system/Debug.h"
#include "logging/Logging.h"
#include "atomic/UseCntPtr.h"
#include "signals/Signal.h"

using namespace ombt;

// main entry point
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    dbgprintf("Create an Signal mask ...\n");
    Signal::Mask sigmask;

    dbgprintf("Set Signal mask to catch SIGINT ...\n");
    sigmask.set(SIGINT);
    dbgprintf("Set Signal mask to catch SIGABRT ...\n");
    sigmask.set(SIGABRT);

    dbgprintf("Create a Signal ...\n");
    UseCntPtr<Signal> psig(new Signal(sigmask));
    MustBeTrue(psig != NULL);

    dbgprintf("Catch Signal signals ...\n");
    signalfd_siginfo sigbuf;
    size_t count = sizeof(sigbuf);
    MustBeTrue(psig->read(&sigbuf, count) != NOTOK);

    switch (sigbuf.ssi_signo)
    {
    case SIGINT:
        dbgprintf("Intercepted Signal SIGINT ...\n");
        break;
    case SIGABRT:
        dbgprintf("Intercepted Signal SIGABRT ...\n");
        break;
    default:
        dbgprintf("Intercepted Unknown Signal ...\n");
    }

    return(0);
}

