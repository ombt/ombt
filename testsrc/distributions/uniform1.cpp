//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include "random/Random.h"
#include "distributions/Uniform.h"
#include "logging/Logging.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 5)
    {
        dbgprintf("usage: %s a b iseed number\n", argv[0]);
        return(2);
    }

    int arg = 0;
    double a = atof(argv[++arg]);
    double b = atof(argv[++arg]);
    unsigned long iseed = atol(argv[++arg]);
    int number = atoi(argv[++arg]);

    Random rng(iseed);
    Uniform urng(a, b, rng);
    double sum = 0.0;

    for (int i=1; i<=number; ++i)
    {
        double value = urng.nextValue();
        dbgprintf("%02d: %10g\n", i, value);
        sum += value;
    }
    dbgprintf("mean = %10g\n", sum/number);

    return(0);
}
