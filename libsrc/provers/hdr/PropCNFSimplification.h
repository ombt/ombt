//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_PROC_CNF_SIMPLIFICATION_H
#define __OMBT_PROC_CNF_SIMPLIFICATION_H

// use CNF simplification on Prop Calc to solve logic problems

// os headers
#include <string>
#include <list>
#include <iostream>

// headers
#include "provers/Strategy.h"

namespace ombt {

// strategy to solve prop calc logic problems
class PropCNFSimplification: public Strategy
{
public:
    // ctors and dtor
    PropCNFSimplification();
    PropCNFSimplification(const Program &);
    PropCNFSimplification(const PropCNFSimplification &src);
    virtual ~PropCNFSimplification();

    // assignment
    PropCNFSimplification &operator=(const Program &program);
    PropCNFSimplification &operator=(const PropCNFSimplification &rhs);

    // inference methods
    virtual int run();

    // print data
    friend std::ostream &operator<<(std::ostream &, 
                                    const PropCNFSimplification &);
};

}

#endif

