//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "system/Debug.h"
#include "logic/EqualityPredCalcTokenizer.h"
#include "logic/EqualityPredCalcParser.h"

using namespace ombt;

void
usage(const std::string &cmd)
{
    std::cerr << "usage: " << cmd 
              << " [-?] [-S] [-T] ]-P] [-I include_dir [-I ...}}"
              << std::endl;
}

int
main(int argc, char *argv[])
{
    EqualityPredCalcTokenizer epct;
    EqualityPredCalcParser epcp;

    for (int c = 0; (c = getopt(argc, argv, "S?I:TP")) != -1 ; )
    {
        switch (c)
        {
        case '?':
            usage(argv[0]);
            return 0;
        case 'I':
            epct.addIncludeDirectory(optarg);
            break;
        case 'T':
            epct.debug(true);
            break;
        case 'P':
            epcp.debug(true);
            break;
        case 'S':
            epcp.enable_semantics(false);
            break;
        default:
            ERRORD("Invalid command option", c, EINVAL);
            return 2;
        }
    }

    Program p;

    for (int arg=optind; arg<argc; ++arg)
    {
        epct.setFile(argv[arg]);
        epcp.parse(epct, p);
        std::cerr << p << std::endl;
    }
    return(0);
}
