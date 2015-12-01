//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "random/Random.h"
#include "logging/Logging.h"

using namespace ombt;

int
main(int argc, char *argv[])
{
    dbgopen(stderr);

    if (argc != 4)
    {
        dbgprintf("usage: %s seed number modulo\n", argv[0]);
        return(2);
    }

    int arg = 0;
    unsigned long seed = atol(argv[++arg]);
    int number = atol(argv[++arg]);
    unsigned long modulo = atol(argv[++arg]);

    std::vector<unsigned long> dist(modulo, 0);

    dbgprintf("Seed   = %lu\n", seed);
    dbgprintf("Number = %d\n", number);
    dbgprintf("Modulo = %d\n", modulo);

    Random rng(seed);

    for (int i=1; i<= number; ++i)
    {
        unsigned long nran = rng.random()%modulo;
        //dbgprintf("%03d: number is ... %lu\n", i, nran);
        dist[nran] += 1;
    }

    dbgprintf("\nDistribution:\n");
    for (int i=0; i<modulo; ++i)
    {
        dbgprintf("%03d: %lu\n", i, dist[i]);
    }

    return(0);
}

