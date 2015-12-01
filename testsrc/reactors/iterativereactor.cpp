//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// simple test for iterative reactor class

// system headers
#include <string>

// local headers
#include "system/Debug.h"
#include "logging/Logging.h"
#include "atomic/UseCntPtr.h"
#include "reactors/IterativeReactor.h"

using namespace ombt;

// main entry point
int
main(int argc, char **argv)
{
    dbgopen(stderr);

    dbgprintf("Create an IterativeReactor ...\n");
    UseCntPtr<IterativeReactor> preactor(new IterativeReactor());
    MustBeTrue(preactor != NULL);

    return(0);
}

