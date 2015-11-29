//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <Debug.h>
#include <logging/Logging.h>
#include <atomic/UseCntBase.h>
#include <atomic/UseCntPtr.h>

using namespace std;
using namespace ombt;

class UseCntDerived: public UseCntBase {
public:
    UseCntDerived(): UseCntBase() { TRACE(); }
    virtual ~UseCntDerived() { TRACE(); }
};

void foobar(UseCntDerived *p)
{
TRACE();
    UseCntPtr<UseCntDerived> puc(p);
TRACE();
    UseCntPtr<UseCntDerived> puc2;
TRACE();
    puc2 = puc;
TRACE();
}

int
main(int argc, char **argv)
{
TRACE();
    UseCntPtr<UseCntDerived> puc;
    UseCntPtr<UseCntDerived> puc2;
    UseCntPtr<UseCntDerived> puc3;
    UseCntPtr<UseCntDerived> puc4;
TRACE();
    puc = new UseCntDerived;
TRACE();
    puc2 = puc;
TRACE();
    puc3 = puc;
TRACE();
    puc4 = puc;
TRACE();
    foobar(puc);
TRACE();
    puc4 = NULL;
TRACE();
    puc3 = NULL;
TRACE();
    puc2 = NULL;
TRACE();
    puc = NULL;
TRACE();
    return(0);
}

