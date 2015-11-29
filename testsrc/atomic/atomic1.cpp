//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Debug.h>
#include <atomic/Atomic.h>

using namespace std;
using namespace ombt;

int
main(int argc, char *argv[])
{
    Atomic<unsigned long>  atomic(1);

    MustBeTrue(atomic.test_and_set(1, 1));

    MustBeTrue(atomic.increment_and_test(2));

    MustBeTrue(atomic.decrement_and_test(1));

    return(0);
}

