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

using namespace ombt;

main(int argc, char *argv[])
{
    std::string program = "";
    for (int arg=1; arg<argc; arg++)
    {
        program += std::string(argv[arg]) + " ";
    }
    DUMP(program);

    PredCalcTokenizer pct;

    pct.setProgram(program);

    for (pct.advanceToNextToken(); 
         pct.currentType() != PredCalcTokenizer::EndOfProgram;
         pct.advanceToNextToken())
    {
        DUMP(pct.currentTypeName());
        DUMP(pct.currentValue());
    }

    return(0);
}
