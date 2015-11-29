
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
    MustBeTrue(!atomic.test_and_set(0, 1));
    MustBeTrue(atomic.test_and_set(1, 0));
    MustBeTrue(!atomic.test_and_set(1, 1));
    MustBeTrue(atomic.test_and_set(0, 1));

    return(0);
}

