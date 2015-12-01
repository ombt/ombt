//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// CNF simplification strategy for prop calc logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "hdr/PropCNFSimplification.h"

namespace ombt {

// ctors and dtor
PropCNFSimplification::PropCNFSimplification():
    Strategy()
{
}

PropCNFSimplification::PropCNFSimplification(const Program &program): 
    Strategy(program)
{
}

PropCNFSimplification::PropCNFSimplification(
    const PropCNFSimplification &src): 
        Strategy(src)
{
}

PropCNFSimplification::~PropCNFSimplification()
{
}

// assignment
PropCNFSimplification &
PropCNFSimplification::operator=(const Program &program)
{
    Strategy::operator=(program);
    return(*this);
}

PropCNFSimplification &
PropCNFSimplification::operator=(const PropCNFSimplification &rhs)
{
    if (this != &rhs)
    {
        Strategy::operator=(rhs);
    }
    return(*this);
}

// proof strategy functions
int
PropCNFSimplification::run()
{
    return error_.ok();
}

// output operators
std::ostream &
operator<<(std::ostream &os, const PropCNFSimplification &p)
{
    return os;
}

}

