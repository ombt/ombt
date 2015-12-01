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

using namespace ombt;

main(int argc, char *argv[])
{
    EqualityPredCalcTokenizer epct;

    std::string program = "";
    for (int arg=1; arg<argc; arg++)
    {
        DUMP(argv[arg]);

        epct.setFile(argv[arg]);

        for (epct.advanceToNextToken(); 
             epct.currentType() != EqualityPredCalcTokenizer::EndOfProgram;
             epct.advanceToNextToken())
        {
            DUMP(epct.currentTypeName());
            DUMP(epct.currentValue());
        }

    }
    return(0);
}
