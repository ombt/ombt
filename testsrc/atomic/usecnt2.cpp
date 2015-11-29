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

class BaseClass: public UseCntBase {
public:
    BaseClass(): UseCntBase() { TRACE(); }
    virtual ~BaseClass() { TRACE(); }
    virtual void WhoAmI() { cout << "I'm a BaseClass" << endl; }
};

class DerivedClass: public BaseClass {
public:
    DerivedClass(): BaseClass() { TRACE(); }
    virtual ~DerivedClass() { TRACE(); }
    virtual void WhoAmI() { cout << "I'm a DerivedClass" << endl; }
};

void foobar(BaseClass *p)
{
TRACE();
    UseCntPtr<BaseClass> puc(p);
TRACE();
    puc->WhoAmI();
TRACE();
    UseCntPtr<BaseClass> puc2;
TRACE();
    puc2 = puc;
TRACE();
}

int
main(int argc, char **argv)
{
TRACE();
    UseCntPtr<DerivedClass> puc;
TRACE();
    UseCntPtr<BaseClass> puc2;
TRACE();
    UseCntPtr<BaseClass> puc3;
TRACE();
    UseCntPtr<BaseClass> puc4;
TRACE();
    puc = new DerivedClass;
TRACE();
    puc2 = new BaseClass;
TRACE();
    puc3 = puc;
TRACE();
    puc4 = puc;
TRACE();
    foobar(puc);
TRACE();
    foobar(puc2);
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

