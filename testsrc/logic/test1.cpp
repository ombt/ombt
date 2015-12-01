//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <string>
#include <vector>
#include <iostream>
#include "system/Debug.h"
#include "logic/LogicTokenizer.h"

using namespace ombt;

main(int argc, char *argv[])
{
    std::string program = "";
    std::vector<std::string> tokens;

    LogicTokenizer lt;

    for (int arg=0; arg<argc; arg++)
    {
        program += std::string(argv[arg]) + " ";
    }

    lt(program, tokens);

    std::vector<std::string>::const_iterator tci = tokens.begin();
    std::vector<std::string>::const_iterator tciend = tokens.end();

    DUMP("dumping all tokens ...");
    for ( ; tci!=tciend; ++tci)
    {
        DUMP(*tci);
    }

    program = "";

    for (int arg=0; arg<argc; arg++)
    {
        program += std::string(argv[arg]) + "|";
    }

    tokens.clear();

    lt(program, tokens, "|");

    tci = tokens.begin();
    tciend = tokens.end();

    DUMP("dumping all tokens ...");
    for ( ; tci!=tciend; ++tci)
    {
        DUMP(*tci);
    }

    return(0);
}
