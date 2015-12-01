//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <string>
#include <vector>
#include <iostream>
#include "system/Debug.h"
#include "logic/PredCalcTokenizer.h"
#include "logic/PredCalcParser.h"

using namespace ombt;

main(int argc, char *argv[])
{
    PredCalcTokenizer pct;
    PredCalcParser pcp;

    int arg=1;

    if ((arg < argc) && (std::string(argv[arg]) == "-d"))
    {
TRACE();
        ++arg;
        pcp.debug(true);
        pct.debug(true);
    }

    std::string program = "";
    for ( ; arg<argc; ++arg)
    {
        DUMP(argv[arg]);
        pct.setFile(argv[arg]);
        pcp.parse(pct);
    }
    return(0);
}
