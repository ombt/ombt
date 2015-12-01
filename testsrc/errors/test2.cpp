//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// simple test for notify class

// system headers
#include <string>
#include <iostream>

// local headers
#include "system/Debug.h"
#include "errors/Errors.h"

using namespace ombt;

// main entry point
int
main(int argc, char **argv)
{
    Errors e;

    for (int ec=-1; ec<=Errors::EC_Last; ++ec)
    {
        std::cout << e.message((Errors::ErrorCodes)ec) << std::endl;
    }

    return(0);
}

