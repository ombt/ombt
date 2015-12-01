//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// DNF simplification strategy for prop calc logic problems

// system includes
#include <string>
#include <list>

// local includes
#include "system/Debug.h"
#include "hdr/PropDNFSimplification.h"

namespace ombt {

// ctors and dtor
PropDNFSimplification::PropDNFSimplification():
    Strategy()
{
}

PropDNFSimplification::PropDNFSimplification(const Program &program): 
    Strategy(program)
{
}

PropDNFSimplification::PropDNFSimplification(
    const PropDNFSimplification &src): 
        Strategy(src)
{
}

PropDNFSimplification::~PropDNFSimplification()
{
}

// assignment
PropDNFSimplification &
PropDNFSimplification::operator=(const Program &program)
{
    Strategy::operator=(program);
    return(*this);
}

PropDNFSimplification &
PropDNFSimplification::operator=(const PropDNFSimplification &rhs)
{
    if (this != &rhs)
    {
        Strategy::operator=(rhs);
    }
    return(*this);
}

// proof strategy functions
int
PropDNFSimplification::run()
{
    return error_.ok();
}

// output operators
std::ostream &
operator<<(std::ostream &os, const PropDNFSimplification &p)
{
    return os;
}

}

