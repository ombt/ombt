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
#include "logic/Program.h"
#include "logic/CNFProgram.h"
#include "logic/DNFProgram.h"
#include "logic/ClausesProgram.h"
#include "provers/Prover.h"

using namespace ombt;

void
usage(const std::string &cmd)
{
    std::cout << "usage: " << cmd 
              << " [-?] [-r] [-s] [-S] [-T] ]-P] [-I include_dir [-I ...}}"
              << std::endl;
}

int
main(int argc, char *argv[])
{
    bool syntax = false;
    bool reconstruct = false;

    EqualityPredCalcTokenizer epct;
    EqualityPredCalcParser epcp;

    for (int c = 0; (c = getopt(argc, argv, "rsS?I:TP")) != -1 ; )
    {
        switch (c)
        {
        case '?':
            usage(argv[0]);
            return 0;
        case 'r':
            reconstruct = true;
            break;
        case 's':
            syntax = true;
            break;
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


    for (int arg=optind; arg<argc; ++arg)
    {
        Program p;

        epct.setFile(argv[arg]);

        epcp.parse(epct, p);
        MustBeTrue(!epcp == false)

        Prover prover;
        prover.setProgram(p);

        if (prover.prove() == OK)
        {
           std::cout << "Program: " << argv[arg] << " PASSED." 
                     << std::endl;
        }
        else
        {
           std::cout << "Program: " << argv[arg] << " FAILED." 
                     << std::endl;
           std::cout << prover.error() << std::endl;
        }

        CNFProgram cnfp(p);

        prover.setProgram(cnfp);
        if (prover.prove() == OK)
        {
           std::cout << "Program: " << argv[arg] << " PASSED." 
                     << std::endl;
        }
        else
        {
           std::cout << "Program: " << argv[arg] << " FAILED." 
                     << std::endl;
           std::cout << prover.error() << std::endl;
        }
    }

    return(0);
}
