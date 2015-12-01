//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_PROVER_H
#define __OMBT_PROVER_H

// prover used to solve logic problems

// os headers
#include <string>
#include <list>
#include <iostream>

// headers
#include "atomic/BaseObject.h"
#include "atomic/ExtUseCntPtr.h"
#include "errors/Errors.h"
#include "logic/Program.h"

namespace ombt {

// prover to solve logic problems
class Prover: public BaseObject
{
public:
    // ctors and dtor
    Prover();
    Prover(const Prover &src);
    ~Prover();

    // assignment
    Prover &operator=(const Prover &rhs);

    // inference methods
    void setProgram(const Program &);
    int prove();

    // misc
    inline bool operator!() const
        { return !error_; }
    inline const Errors &error() const
        { return error_; }

    // print data
    friend std::ostream &operator<<(std::ostream &, const Prover &);

protected:
    // internal data
    Errors error_;
    Program program_;
};

}

#endif

