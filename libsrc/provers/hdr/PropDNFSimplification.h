//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_PROC_DNF_SIMPLIFICATION_H
#define __OMBT_PROC_DNF_SIMPLIFICATION_H

// use DNF simplification on Prop Calc to solve logic problems

// os headers
#include <string>
#include <list>
#include <iostream>

// headers
#include "provers/Strategy.h"

namespace ombt {

// strategy to solve prop calc logic problems
class PropDNFSimplification: public Strategy
{
public:
    // ctors and dtor
    PropDNFSimplification();
    PropDNFSimplification(const Program &);
    PropDNFSimplification(const PropDNFSimplification &src);
    virtual ~PropDNFSimplification();

    // assignment
    PropDNFSimplification &operator=(const Program &program);
    PropDNFSimplification &operator=(const PropDNFSimplification &rhs);

    // inference methods
    virtual int run();

    // print data
    friend std::ostream &operator<<(std::ostream &, 
                                    const PropDNFSimplification &);
};

}

#endif

