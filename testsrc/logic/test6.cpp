//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <string>
#include <vector>
#include <iostream>
#include "system/Debug.h"
#include "logic/EqualityPredCalcTokenizer.h"
#include "logic/EqualityPredCalcParser.h"

using namespace ombt;

main(int argc, char *argv[])
{
    EqualityPredCalcTokenizer epct;
    EqualityPredCalcParser epcp;

    int arg=1;

    if ((arg < argc) && (std::string(argv[arg]) == "-d"))
    {
TRACE();
        ++arg;
        epcp.debug(true);
        epct.debug(true);
    }

    Program p;

    std::string program = "";
    for ( ; arg<argc; ++arg)
    {
        DUMP(argv[arg]);
        epct.setFile(argv[arg]);
        epcp.parse(epct, p);
    }
    return(0);
}
