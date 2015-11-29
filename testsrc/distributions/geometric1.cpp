//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <random/Random.h>
#include <distributions/Geometric.h>
#include <logging/Logging.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
    dbgopen(stderr);

    if (argc != 4)
    {
        dbgprintf("usage: %s p iseed number\n", argv[0]);
        return(2);
    }

    int arg = 0;
    double p = atof(argv[++arg]);
    MustBeTrue(0 <= p && p <= 1.0);
    unsigned long iseed = atol(argv[++arg]);
    int number = atoi(argv[++arg]);

    Random rng(iseed);
    Geometric urng(p, rng);
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
